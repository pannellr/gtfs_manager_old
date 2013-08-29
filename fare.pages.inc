<?php

function gtfs_manager_fare_form($form, &$form_state, $fare = null) {

  //set the id to identify this as the route edit form
  $form['#id'] = 'fare-form';

  $form['#fare'] = $fare;
  $form_state['fare'] = $fare;

  //Text field for route short name
  $form['gtfs_manager_route_fare_price'] = array(
    '#type' => 'textfield',
    '#title' => t('Fare price'),
    '#description' => t('3.00 (no symbols please)'),
    '#default_value' => $fare->price,
    '#size' => 10,
  );

  $form['gtfs_manager_currency_type_id'] = array(
    '#type' => 'select',
    '#title' => t('Currency Type'),
    '#default_value' => $fare->currency_type_id,
    '#options' => fetch_currency_types(),
    '#required' => TRUE,
  );

  $form['gtfs_manager_transfers'] = array(
    '#type' => 'select',
    '#title' => t('Transfer allowance'),
    '#default_value' => $fare->transfers,
    '#options' => array('0' => t('Unlimited transfers'), '1' => t('One Transfer'), '2' => t('Two Transfers')),
    '#required' => TRUE,
  );

  $form['gtfs_manager_payment_method'] = array(
    '#type' => 'radios',
    '#title' => 'Payment Method',
    '#options' =>
    array(0 => t('Fare is paid on board'),
	  1 => t('Fare is paid before boarding')
    ),
    '#default_value' => $fare->payment_method,
  );

  // Add the buttons
  $form['buttons'] = array();
  $form['buttons']['#weight'] = 100;
  $form['buttons']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save Fare'),
    '#weight' => 5,
    '#submit' => array('gtfs_manager_fare_form_submit'),
  );

  if (!empty($route->rid)) {
    $form['buttons']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete'),
      '#weight' => 15,
      '#submit' => array('gtfs_manager_fare_form_delete_submit'),
    );
  }

  $form['#validate'][] = 'gtfs_manager_fare_form_validate';

  //allow other fields to be added
  field_attach_form('fare', $fare, $form, $form_state);

  return $form;
}

function fetch_currency_types() {
  $result = db_query('select * from {currency_types}');

  $options = array();

  foreach ($result as $type) {
    $options[$type->currency_type_id] = $type->currency_type_code;
  }

  return $options;
}

function gtfs_manager_fare_form_validate($form, &$form_state) {

}

/**
 * Implements hook_form_submit() for route
 */
function gtfs_manager_fare_form_submit($form, &$form_state) {
  global $user;

  $fare = $form_state['fare'];


  // if this is a new record
  if (empty($route->created_by)) {
    $fare->created_by = $user->name;
  }

  $fare->updated_by = $user->name;


  $fare->price = $form_state['values']['gtfs_manager_route_fare_price'];
  $fare->currency_type_id = $form_state['values']['gtfs_manager_currency_type_id'];
  $fare->transfers = $form_state['values']['gtfs_manager_transfers'];
  $fare->payment_method = $form_state['values']['gtfs_manager_payment_method'];

  //notify field widgets
  field_attach_submit('fare', $fare, $form, $form_state);

  //Save
  fare_save($fare);

  //gtfs_manager_service_area_points

  drupal_set_message(t('Fare saved'));

  $form_state['redirect'] = 'agencies/';

}

function gtfs_manager_fare_form_delete_submit($form, &$form_state) {
  $destination = array();
  if (isset($_GET['destination'])) {
    $destinaltion = drupal_get_destination();
    unset($_GET['destination']);
  }

  $fare = $form['#fare'];
  $form_state['redirect'] = array('fare/' . $fare->fare_id . '/delete',
    array('query' => $destination));
}

function gtfs_manager_show_fares() {

  $query = db_select('fare_attributes', 'f')
    ->fields('f', array('fare_id', 'price', 'payment_method'));

  $query->join('currency_types', 'ct', 'ct.currency_type_id = f.currency_type_id');

  $result = $query->fields('ct', array('currency_type_name', 'currency_type_code'))
    ->execute();

  $fares = array();

  while ($fare = $result->fetchObject()) {
    $fare->edit_link = l('Edit', 'fare/' . $fare->fare_id . '/edit');
    $fares[] = $fare;
  }

  $variables['fares'] = $fares;


  return theme('gtfs_manager_show_fares', $variables);

}

function fare_page_edit($fare) {
  drupal_set_title(t('<em>Edit fare</em>'));
  return drupal_get_form('gtfs_manager_fare_form', $fare);
}



function route_fare_form($form, &$form_state, $route) {
  $combinations = fetch_stop_combinations($route->rid);

  //set the id to identify this as the route edit form
  $form['#id'] = 'route-fares-form';

  $form['#route'] = $route;
  $form_state['route'] = $route;
  $form_state['combinations'] = $combinations;

  $fare_options = fetch_fares();

  foreach ($combinations as $combination) {

    $form['gtfs_manager_zone_fare-' . $combination->from_sid . '_' . $combination->to_sid] = array(
      '#type' => 'select',
      '#default_value' => $combination->fare_id,
      '#options' => $fare_options,
    );
  }

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save Fares'),
    '#weight' => 5,
    '#submit' => array('gtfs_manager_route_fares_form_submit'),
  );

  $form['#attached']['css'] = array(
    drupal_get_path('module', 'gtfs_manager') . '/fare_rules_form_theme.css',
  );

  $form['#validate'][] = 'gtfs_manager_route_fares_form_validate';

  return $form;
}

function gtfs_manager_route_fares_form_validate($form, &$form_state) {

}

function gtfs_manager_route_fares_form_submit($form, &$form_state) {
  global $user;

  $combinations = $form_state['combinations'];

  foreach ($combinations as $combination) {

    $fare_id = $form_state['values']['gtfs_manager_zone_fare-'
				     . $combination->from_sid
				     . '_'
				     . $combination->to_sid];

    if (!empty($fare_id)) {

      $fare_rule = entity_get_controller('fare_rule')->create();

      $fare_rule->created_by = $user->name;
      $fare_rule->updated_by = $user->name;

      $fare_rule->fare_id = $fare_id;

      //create zone_ids for from_stop and to_stop if they do not exist

      if (!$fare_rule->origin_id = get_zone_for($combination->from_sid)) {
        $fare_rule->origin_id = create_zone_for($combination->from_sid);
      }

      if (!$fare_rule->destination_id = get_zone_for($combination->to_sid)) {
        $fare_rule->destination_id = create_zone_for($combination->to_sid);
      }

      fare_rule_save($fare_rule);
    }
  }
  drupal_set_message('Fare Rule(s) saved');
}


function fetch_stop_combinations($rid) {
  $query =<<<SQL
select  s.sid as from_sid
  ,  s.stop_name as from_stop_name
  ,  s.zone_id as from_zone_id
  ,  st.stop_sequence as from_stop_sequence
  ,  s2.sid as to_sid
  ,  s2.stop_name as to_stop_name
  ,  s2.zone_id as to_zone_id
  ,  st2.stop_sequence as to_stop_sequence
  ,  ifnull(fa.fare_id, 0) as fare_id
  ,  fa.price
  ,  fa.payment_method
  ,  ct.currency_type_code
from routes r
join trips t
  on t.rid = r.rid
join stop_times st
  on st.tid = t.tid
join stops s
  on s.sid = st.sid
join trips t2
  on t2.rid = r.rid
join stop_times st2
  on st2.tid = t2.tid
join stops s2
  on s2.sid = st2.sid
  and st2.stop_sequence > st.stop_sequence
left join fare_rules fr
  on fr.origin_id = s.zone_id
  and fr.destination_id = s2.zone_id
left join fare_attributes fa
  on fa.fare_id = fr.fare_id
left join currency_types ct
  on ct.currency_type_id = fa.currency_type_id
    where r.rid = :rid
order by from_stop_sequence, to_stop_sequence;
SQL;

  $result = db_query($query, array(':rid' => $rid));

  $combinations = array();

  while ($combination = $result->fetchObject()) {
    $combination->payment_method_string =
      ($combination->payment_method == 0)
      ? 'Pay onboard'
      : 'Pay before boarding';
    $combinations[] = $combination;
  }

  return $combinations;
}



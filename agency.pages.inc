<?php
/**
 * Form builder; create and display the form for a new agency
 */
function gtfs_manager_agency_form($form, &$form_state, $agency = null) {
  $agency_fare_types = array(
    0 => t('All agency origins and destinations'),
    1 => t('Agency zones'),
    2 => t('All route origins and destinations'),
    3 => t('Service area'),
  );

  //Set the id to identify this as the agency edit form
  $form['#id'] = 'agency-form';

  // save the agency for later in case we need it
  $form['#agency'] = $agency;
  $form_state['agency'] = $agency;

  //Text field for agency name
  $form['gtfs_manager_agency_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Transporter name'),
    '#default_value' => stripslashes($agency->agency_name),
    '#description' => t('The name of the transit service provider'),
    '#size' => 40,
    '#required' => TRUE,
  );

  //text field for agency url
  $form['gtfs_manager_agency_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Transporter website url'),
    '#default_value' => $agency->agency_url,
    '#description' => t('The url of the transit service provider'),
    '#size' => 40,
  );

  //textfield for agency phone number
  $form['gtfs_manager_agency_phone'] = array(
    '#type' => 'textfield',
    '#title' => t('Transporter phone number'),
    '#default_value' => $agency->agency_phone,
    '#description' => t('The phone number of the transit service provider'),
    '#size' => 40,
  );

  //textfield for agency email
  $form['gtfs_manager_agency_email'] = array(
    '#type' => 'textfield',
    '#title' => t('Transporter email'),
    '#default_value' => $agency->agency_email,
    '#description' => t('The contact email for this transit service provider'),
    '#size' => 40,
  );

  $form['gtfs_manager_agency_time_zone_id'] = array(
    '#type' => 'select',
    '#title' => t('Time zone'),
    '#default_value' => $agency->time_zone_id,
    '#options' => fetch_time_zones(),
  );

  //text field for agency fare url
  $form['gtfs_manager_agency_fare_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Transporter fare url'),
    '#default_value' => $agency->agency_fare_url,
    '#description' => t('The url for this transporters fare information.  Will be displayed if fare information is not present.'),
    '#size' => 40,
  );

  //Select field for fare type
  $form['gtfs_manager_agency_fare_type'] = array(
    '#type' => 'select',
    '#title' => t('Choose fare type'),
    '#default_value' => $agency->agency_fare_type,
    '#options' => $agency_fare_types,
    '#description' => t('Determines the type of fare entry form that will be shown for this agency.'),
  );

  //text area for agency information
  $form['gtfs_manager_agency_info'] = array(
    '#type' => 'textarea',
    '#title' => t('Agency Information'),
    '#description' => t('This would be a good place to include any extra information that you would like to share with users.'),
    '#default_value' => $agency->agency_info,
    '#rows' => 12,
    '#cols' => 20,
  );

  // Add the buttons
  $form['buttons'] = array();
  $form['buttons']['#wieght'] = 100;
  $form['buttons']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save Agency'),
    '#weight' => 5,
    '#submit' => array('gtfs_manager_agency_form_submit'),
  );

  //Add a delete button if user is editting
  if (!empty($agency->aid)) {
    $form['buttons']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete'),
      '#weight' => 15,
      '#submit' => array('gtfs_manager_agency_form_delete_submit'),
    );
  }

  $form['#validate'][] = 'gtfs_manager_agency_form_validate';

  //allow other fields to be added to form
  field_attach_form('agency', $agency, $form, $form_state);

  return $form;
}

/**
 * Implements hook_validate() for agency submissions
 */
function gtfs_manager_agency_form_validate($form, &$form_state) {
  $agency = $form_state['agency'];

  //validate phone


  //validate urls

  //validate email
}

/**
 * Submit hook for agency form
 */
function gtfs_manager_agency_form_submit($form, &$form_state) {
  global $user;

  $agency = &$form_state['agency'];

  // if this is a new record
  if (empty($agency->created_by)) {
    $agency->created_by = $user->name;
  }

  $agency->updated_by = $user->name;
  $agency->agency_name = addslashes($form_state['values']['gtfs_manager_agency_name']);
  $agency->agency_url = $form_state['values']['gtfs_manager_agency_url'];
  $agency->agency_phone = $form_state['values']['gtfs_manager_agency_phone'];
  $agency->agency_email = $form_state['values']['gtfs_manager_agency_email'];
  $agency->agency_info = addslashes($form_state['values']['gtfs_manager_agency_info']);
  $agency->agency_fare_type = $form_state['values']['gtfs_manager_agency_fare_type'];
  $agency->agency_fare_url = $form_state['values']['gtfs_manager_agency_fare_url'];
  $agency->time_zone_id = $form_state['values']['gtfs_manager_agency_time_zone_id'];

  //notify field widgets
  field_attach_submit('agency', $agency, $form, $form_state);

  //Save
  agency_save($agency);

  drupal_set_message(t('Agency saved'));

  $form_state['redirect'] = 'agencies';

}

function gtfs_manager_agency_form_delete_submit($form, &$form_state) {
  $destination = array();
  if (isset($_GET['destination'])) {
    $destinaltion = drupal_get_destination();
    unset($_GET['destination']);
  }

  $agency = $form['#agency'];
  $form_state['redirect'] = array('agency/' . $agency->aid . '/delete',
    array('query' => $destination));
}

function agency_page_view($agency, $view_mode = 'full') {
  // clear out any existing content
  $agency->content = array();

  if ($view_mode == 'full') {
    $agency->content['title'] = array(
      '#markup' => filter_xss($agency->agency_name),
      '#weight' => -5,
    );

  }

  field_attach_prepare_view('agency', array($agency->aid => $agency), $view_mode);
  entity_prepare_view('agency', array($agency->aid => $agency));
  $agency->content += field_attach_view('agency', $agency, $view_mode);

  return $agency->content;

}

function agency_page_edit($agency) {
  drupal_set_title(t('<em>Edit @name route</em>', array('@name'=> $agency->agency_name)), PASS_THROUGH);
  return drupal_get_form('gtfs_manager_agency_form', $agency);
}

function agency_delete_confirm($form, &$form_state, $agency) {
  $form['#agency'] = $agency;
  $form['aid'] = array('#type' => 'value', '#value' => $agency->aid);
  return confirm_form($form,
    t('Are you sure you want to delete the route: %name?',
      array('%name' => $agency->agency_name)),
    'agency/' . $agency->aid,
    t('This action cannot be undone.'),
    t('Delete'),
    t('Cancel')
  );
}

function agency_delete_confirm_submit($form, &$form_state) {
  if ($form_state['values']['confirm']) {
    $agency = agency_load($form_state['values']['aid']);
    agency_delete($form_state['values']['aid']);
  }

  watchdog('agency', '@name deleted.', array('@name' => $agency->agency_name));

  drupal_set_message(t('The agency %name has been deleted.',
    array('%name' => $agency->agency_name)
  ));

  $form_state['redirect'] = 'agencies/view';
}

/**
 * List all agencies in database
 */
function gtfs_manager_all_agencies_view() {
  $variables = array();
  $agencies = array();

  $result = db_query("select * from agencies order by agency_name");

  while ($agency = $result->fetchObject()) {
    $agency->features = fetch_agency_features($agency->aid);
	$agency->options[] = l('Routes', 'agency/' . $agency->aid . '/routes');

	if (user_access('administer gtfs')) {
	  $agency->options[] = l('Edit' , 'agency/' . $agency->aid . '/edit');
	  $agency->options[] = l('Features', 'agency/' . $agency->aid . '/feature/add');
	  if ($agency->agency_fare_type == 0) {
	    $agency->options[] = l('Fares', 'agency/' . $agency->aid . '/fares/matrix');
	    $agency->options[] = l('Upload Fares', 'agency/' . $agency->aid . '/fares/upload');
	  }  elseif ($agency->agency_fare_type == 1) {
	    $agency->options[] = l('Fares', 'agency/' . $agency->aid . '/fares/add');
	  }
	}
	$agencies[] = $agency;
  }

  $variables['links'][] = l('Create New Transporter', 'agency/add');

  $variables['agencies'] = !empty($agencies) ? $agencies : '';

  return theme('gtfs_manager_agencies', $variables);

}

function agency_routes_page_view($agency, $view_mode = 'full') {
  $variables = array();

  $routes = route_load_multiple(FALSE, array('aid' => $agency->aid));

  $variables['links'][] = l(t('Create Route'), 'agency/' . $agency->aid . '/route/add');
  $variables['links'][] = l(t('Back to transporters'), 'agencies/view');

  foreach ($routes as $route) {

    if ($route->route_service_type = 'point-to-point') {
      $route->links[] = l(t('Stops'), 'route/' . $route->rid . '/stops');
    }

    if (user_access('administer gtfs')) {
      $route->links[] = l(t('Edit'), 'route/' . $route->rid . '/edit');
      $route->links[] = l(t('Features'), 'agency/' . $agency->aid .'/route/' . $route->rid . '/feature/add');

      if ($route->route_service_type = 'point-to-point') {
        $route->links[] = l(t('Fares'), 'route/' . $route->rid . '/fares/add');
        $route->links[] = l(t('Upload Stops'), 'route/' . $route->rid . '/upload');
      }
    }
  }

  $variables['routes'] = $routes;

  return theme('gtfs_manager_agency_routes', $variables);
}

function gtfs_manager_agency_detail($agency) {
  $variables['agency'] = $agency;
  return theme('gtfs_manager_agency_detail', $variables);
}

/**
 * Form builder; create and display the form for a new agency service feature
 */
function gtfs_manager_agency_feature_form($form, &$form_state, $agency_service_feature, $agency) {
  $agency->service_feature_ids = fetch_features('agency', $agency->aid, TRUE);

  //Set the id to identify this as the agency edit form
  $form['#id'] = 'agency-service-feature-form';

  // save the agency and service feature objects for later
  $form['#agency'] = $agency;
  $form_state['agency'] = $agency;
  $form_state['agency_service_feature'] = $agency_service_feature;

  $form['create_link'] = array(
    '#markup' => l('Create a new feature', 'service_feature/new')
  );

  //checkboxes for all features
  $form['gtfs_manager_agency_service_feature_ids'] = array(
    '#type' => 'checkboxes',
    '#options' => fetch_service_features(),
    '#title' => t('Select features provided by this agency'),
    '#default_value' => $agency->service_feature_ids
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Update Service Features'),
    '#weight' => 5,
    '#submit' => array('gtfs_manager_agency_service_features_form_submit'),
  );

  $form['#validate'][] = 'gtfs_manager_agency_service_features_form_validate';

  return $form;
}



function gtfs_manager_agency_service_features_form_validate($form, &$form_state) {

}

function gtfs_manager_agency_service_features_form_submit($form, &$form_state) {
  global $user;

  $service_feature_ids = $form_state['agency']->service_feature_ids;

  $agency_service_feature = $form_state['agency_service_feature'];

  // if this is a new record
  if (empty($agency_service_feature->created_by)) {
    $agency_service_feature->created_by = $user->name;
  }

  $agency_service_feature->updated_by = $user->name;
  $agency_service_feature->aid = $form_state['agency']->aid;

  //make a copy so we can loop through and save the different features
  $copy = $agency_service_feature;


  foreach ($form_state['values']['gtfs_manager_agency_service_feature_ids'] as $key => $value) {

    //if it is selected
    if ($value != 0) {
      $copy->service_feature_id = $key;
      //Save
      agency_service_feature_save($copy);
      // remove selected keys and delete leftovers later
      unset($service_feature_ids[$key]);
    }
  }

  if (sizeof($service_feature_ids) != 0) {
      $query = 'select agency_service_feature_id from agency_service_features where aid = :aid and service_feature_id in (:sfids)';
      $result = db_query($query, array(':aid' => $agency_service_feature->aid, ':sfids' => implode(',', $service_feature_ids)));
      while ($id = $result->fetchField()) {
        agency_service_feature_delete(array($id));
      }
  }

  drupal_set_message(t('Agency Service Feature(s) updated'));

  $form_state['redirect'] = 'agencies/';

}

/**
 * Save a new or updated service feature
 *
 * @param $service_feature a populated agency object
 */
function agency_service_feature_save($agency_service_feature) {
  return entity_get_controller('agency_service_feature')->save($agency_service_feature);
}

/**
 * Save a new or updated service feature
 *
 * @param $service_feature a populated agency object
 */
function agency_service_feature_delete($agency_service_feature_id) {
  return entity_get_controller('agency_service_feature')->delete($agency_service_feature_id);
}

function fetch_agency_features($aid) {
  return fetch_features('agency', $aid);
}

/**
 * Form builder; create and display the form for agency fares
 */
function gtfs_manager_agency_fares_form($form, &$form_state, $agency){
  //fetch all routes for this agency
  $routes = fetch_agency_zones($agency, FALSE);
  $route_options = array();

  foreach ($routes as $route) {
    $route_options[$route->rid] = $route->route_long_name;
  }

  $variables['zones'] = fetch_agency_zones($agency, TRUE);

  $form['active_transfers_report'] = array(
    '#markup' => theme('gtfs_manager_active_zones', $variables),
  );

  $form['#id'] = 'agency_fares_form';
  $form['#agency'] = $agency;
  $form_state['agency'] = $agency;

   $form['gtfs_manager_fare_id'] = array(
      '#type' => 'select',
      '#title' => t('Select a fare for this zone'),
      '#options' => fetch_fares(),
   	  '#required' => TRUE,
    );

  //display routes as checkboxes
  $form['gtfs_manager_routes'] = array(
    '#type' => 'checkboxes',
    '#options' => $route_options,
    //'#default_value' => $stop->days,
    '#title' => t('Choose route you would like to group under this fare.'),
  );

$form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save Fare group'),
    '#weight' => 5,
    '#submit' => array('gtfs_manager_agency_fares_form_form_submit'),
  );


  $path = drupal_get_path('module', 'gtfs_manager');
  // Attach the CSS and JS to the form
  $form['#attached'] = array('css' => array(
					    'type' => 'file',
					    'data' => $path
					    . '/active_zones_theme.css',
					    ),

			     );

  $form['#validate'][] = 'gtfs_manager_agency_fares_form_validate';

  return $form;
}

function gtfs_manager_agency_fares_form_validate($form, &$form_state) {

}

function gtfs_manager_agency_fares_form_form_submit($form, &$form_state) {
  $agency = $form_state['agency'];

  //create fare rule object
  $fare_rule = entity_get_controller('fare_rule')->create();
  $fare_rule->fare_id = $form_state['values']['gtfs_manager_fare_id'];
  $zone_id = generate_zone_id();

  //loop through selected routes
  foreach ($form_state['values']['gtfs_manager_routes'] as $key => $value) {
    // if the route is selected then loop through its stops
    if ($value) {
      $stops = stop_load_multiple(array(), array('rid', $key, '='));
      // if the route is selected then loop through its stops
      foreach ($stops as $stop) {
        create_zone_for($stop->sid, $zone_id);
      }
    }
  }

  //save the fare_rule
  $fare_rule->contains_id = $zone_id;

  fare_rule_save($fare_rule);

  drupal_set_message(t('Zone successfully created'));

  $form_state['redirect'] = 'agency/' . $agency->aid . '/fares/add';

}
/*
 * returns agency zones
 *
 * @param $agency an agency object
 * @param $active, boolean TRUE returns active zones while false returns inactive zones
 */
function fetch_agency_zones($agency, $active) {
  $zones = array();
  $order = ' order by fr.contains_id';
  $having = $active ? ' having contains_id is not null and contains_id != 0;' : ' having contains_id is null or contains_id = 0;';
  $args[':aid'] = $agency->aid;

  $query =<<<SQL
select a.aid
  ,  r.rid
  ,  r.route_long_name
  ,  fr.contains_id
  ,  fa.price
from agencies a
join routes r
  on r.aid = a.aid
join trips t
  on t.rid = r.rid
join stop_times st
  on st.tid = t.tid
join stops s
  on s.sid = st.sid
left join fare_rules fr
  on fr.contains_id = s.zone_id
left join fare_attributes fa
  on fa.fare_id = fr.fare_id
where a.aid = :aid
group by a.aid, r.rid
SQL;

  $result = db_query($query . $having . $order, $args);

  while ($zone = $result->fetchObject()) {
    $zones[] = $zone;
  }
  return $zones;
}

function gtfs_manager_agency_zone_delete_confirm($form, &$form_state, $agency, $zone_id) {
  $form_state['zone_id'] = $zone_id;
  $form_state['agency'] = $agency;

  return confirm_form($form,
    t('Are you sure you want to delete this zone?'),
    'agency_zones/' . $zone_id,
    t('This action cannot be undone.'),
    t('Delete'),
    t('Cancel')
  );
}

function gtfs_manager_agency_zone_delete_confirm_submit($form, &$form_state) {
  $zone_id = $form_state['zone_id'];
  $agency = $form_state['agency'];

  if ($form_state['values']['confirm']) {
    remove_fare_rules_for($zone_id);
    remove_stop_zone($zone_id);
  }

  watchdog('agency zone', 'Zone: @zone_id deleted.', array('@zone_id' => $zone_id));

  drupal_set_message(t('Zone deleted.'));

  $form_state['redirect'] = 'agency/' . $agency->aid . '/fares/add';
}

function gtfs_manager_agency_zone_route_delete_confirm($form, &$form_state, $agency, $route) {
  $form_state['route'] = $route;
  $form_state['agency'] = $agency;

  return confirm_form($form,
    t('Are you sure you want to remove the route :route from this zone?', array(':route' => $route->route_long_name)),
    'agency_zones/' . $route->rid,
    t('This action cannot be undone.'),
    t('Delete'),
    t('Cancel')
  );
}

function gtfs_manager_agency_zone_route_delete_confirm_submit($form, &$form_state) {
  $route = $form_state['route'];
  $agency = $form_state['agency'];

  if ($form_state['values']['confirm']) {
    remove_route_zone($route);
  }

  watchdog('route removed from zone', 'Route: @rid deleted.', array('@zone_id' => $route->rid));

  drupal_set_message(t('Route removed from zone.'));

  $form_state['redirect'] = 'agency/' . $agency->aid . '/fares/add';
}

function gtfs_manager_agency_fares_matrix_form($form, &$form_state, $agency) {
  //$form_state['no_cache'] = TRUE;
  $combinations = fetch_agency_stop_combinations($agency->aid);
  $form['#id'] = 'agency-fare-matrix';

  $form['#agency'] = $agency;
  $form_state['agency'] = $agency;
  $form_state['combinations'] = $combinations;

  $fare_options = fetch_fares();

  foreach ($combinations as $combination) {
    $combination->fare_id = fetch_fare_id_for($combination, $agency->aid);
    $name = str_replace(' ', '|', trim($combination->from_stop_name)) . '_' . str_replace(' ', '|' , trim($combination->to_stop_name));
    $form['gtfs_manager_zone_fare:' . $name] = array(
      '#type' => 'select',
      '#default_value' => $combination->fare_id,
      '#options' => $fare_options,
      '#prefix' => '<div id="zone-fare-' . str_replace(' ', '', $combination->from_stop_name) . str_replace(' ', '' , $combination->to_stop_name) . '">',
      '#suffix' => '</div>',
      '#ajax' => array(
        'callback' => 'gtfs_manager_agency_fares_matrix_form_ajax_submit',
        'wrapper' => $name,
        'method' => 'replace',
        'effect' => 'fade'
      ),
    );
  }

  $form['#attached']['css'] = array(
    drupal_get_path('module', 'gtfs_manager') . '/fare_rules_form_theme.css',
  );

   $form['#attached']['js'] = array(
    drupal_get_path('module', 'gtfs_manager') . '/fares_matrix.js',
  );

  return $form;

}

/*
function gtfs_manager_agency_fares_matrix_form_validate($form, &$form_state){
  //watchdog('form_state', $form_state);
}
*/

function gtfs_manager_agency_fares_matrix_form_submit($form, &$form_state){
  global $user;

  $combinations = $form_state['combinations'];
  $agency = $form_state['agency'];
  $form_state['updates'] = array();

  foreach ($combinations as $combination) {

    $fare_id = $form_state['values']['gtfs_manager_zone_fare:' . str_replace(' ', '|', $combination->from_stop_name) . '_' . str_replace(' ', '|' , $combination->to_stop_name)];

    if ($fare_id) {
      $fare_rule = entity_get_controller('fare_rule')->create();
      $fare_rule->created_by = $user->name;
      $fare_rule->updated_by = $user->name;
      $fare_rule->fare_id = $fare_id;

      //fetch all of the stop_ids associated with the given name
      $from_sids = fetch_stop_ids($combination->from_stop_name, $agency->aid);
      $to_sids = fetch_stop_ids($combination->to_stop_name, $agency->aid);

      for ($i = 0; $i < sizeof($from_sids); $i++) {
        for ($j = 0; $j < sizeof($to_sids); $j++) {

          //create zone_ids for from_stop and to_stop if they do not exist
          if (!$fare_rule->origin_id = get_zone_for($from_sids[$i])) {
            $fare_rule->origin_id = create_zone_for($from_sids[$i]);
          }

          if (!$fare_rule->destination_id = get_zone_for($to_sids[$j])) {
            $fare_rule->destination_id = create_zone_for($to_sids[$j]);
          }
        }
      }
    }
  }
  drupal_set_message('Fare Rule(s) saved');

}


function gtfs_manager_agency_fares_matrix_form_ajax_submit($form, &$form_state){
  global $user;
  $element = $form_state['triggering_element'];
  $fare_id = $element['#value'];
  $agency = $form_state['agency'];

  $strings = explode(':', $element['#name']);
  $stop_names = explode('_', $strings[1]);

  preg_replace('/|/', '/\s/' , $stop_names[0]);
  preg_replace('/|/', '/\s/' , $stop_names[1]);

  if ($fare_id && user_access('administer gtfs')) {  //be sure to test acess so that not just anybody can write a record tokens would make it even better
    $fare_rule = entity_get_controller('fare_rule')->create();
    $fare_rule->created_by = $user->name;
    $fare_rule->updated_by = $user->name;
    $fare_rule->fare_id = $fare_id;
    $fare_rule_opposite = clone $fare_rule;

    //fetch all of the stop_ids associated with the given name
    $from_sids = fetch_stop_ids($stop_names[0], $agency->aid);
    $to_sids = fetch_stop_ids($stop_names[1], $agency->aid);

    for ($i = 0; $i < sizeof($from_sids); $i++) {
      for ($j = 0; $j < sizeof($to_sids); $j++) {

        //create zone_ids for from_stop and to_stop if they do not exist
        if (!$fare_rule->origin_id = get_zone_for($from_sids[$i])) {
          $fare_rule->origin_id = create_zone_for($from_sids[$i]);
        }

        if (!$fare_rule->destination_id = get_zone_for($to_sids[$j])) {
          $fare_rule->destination_id = create_zone_for($to_sids[$j]);
        }

        fare_rule_save($fare_rule);

        //create a record with the same fare and swapped zones
        $fare_rule_opposite->origin_id = $fare_rule->destination_id;
        $fare_rule_opposite->destination_id = $fare_rule->origin_id;

        fare_rule_save($fare_rule_opposite);
      }
    }
  }
}


function fetch_agency_stop_combinations($aid) {
    $query =<<<SQL
select s.stop_name
from agencies a
join routes r
  on r.aid = a.aid
join trips t
  on t.rid = r.rid
join stop_times st
  on st.tid = t.tid
join stops s
  on s.sid = st.sid
where a.aid = :aid
group by stop_name
order by stop_name
SQL;

  $result = db_query($query, array(':aid' => $aid));

  $stop_names = array();

  while ($stop_name = $result->fetchField()) {
    $stop_names[] = $stop_name;
  }

  $combinations = array();
  $skip = 1;
  for($i = 0; $i < sizeof($stop_names); $i++) {
    for ($j = $skip; $j < sizeof($stop_names); $j++){
      $combination = (object) array('from_stop_name' => $stop_names[$i], 'to_stop_name' => $stop_names[$j]);
      #$combination->fare_id = fetch_fare_id_for ($combintation, $aid);
      $combinations[] = $combination;
    }
    $skip++;
  }

  return $combinations;
}

function gtfs_manager_agency_fares_upload_form($form, &$form_state, $agency, $download = FALSE) {
  $form_state['agency'] = $agency;

  if ($download) {
    gtfs_manager_agency_fares_download($agency);
  }

  $form['gtfs_manager_agency_fares_file_upload'] = array(
    '#title' => t('Upload fares csv file'),
    '#type'  => 'file',
    '#prefix' => l('Download CSV', 'agency/' . $agency->aid . '/fares/download'),
    '#progress_indicator' => 'bar',
  );


  $form['submit'] = array(
    '#type'  =>  'submit',
    '#value'  =>  'Upload'
  );

  return $form;
}

function gtfs_manager_agency_fares_upload_form_validate($form, &$form_state) {
 $agency = $form_state['agency'];

  $file = file_save_upload('gtfs_manager_agency_fares_file_upload', array(
            'file_validate_extensions' => array('csv'),
            FALSE,
            FILE_EXISTS_REPLACE
  ));

  if ($file) {
    if ($file = file_move($file, 'public://', FILE_EXISTS_REPLACE)) {
      $form_state['values']['file'] = $file;
    } else {
      form_set_error('file', t('Failed to write the uploaded file to file folder'));
    }
  } else {
    form_set_error('file', t('No file was uploaded.'));
  }

}

function gtfs_manager_agency_fares_upload_form_submit($form, &$form_state) {
  global $user;
  $agency = $form_state['agency'];
  $skip = 2;
  $file = $form_state['values']['file'];
  unset($form_state['values']['file']);
  $file->status = FILE_STATUS_PERMANENT;
  file_save($file);

  //create dictionary for stop_names => array(sids) !important for upload speed
  $stops = fetch_stops($agency->aid);
  $stop_dictionary = array();

  foreach($stops as $stop) {
    $stop_dictionary[trim($stop->stop_name)][] = $stop->sid;
  }

  //load the file into the database
  if (($fh = fopen(file_create_url($file->uri), 'r')) != FALSE) {
    $skip = 2;
    $cols = array();
    while (($data = fgetcsv($fh, 1000, ",")) != FALSE) {
      //grab the columns from the first row
      if (empty($cols)) {
          $cols = $data;
      } else {
        //the origin will always be the first element
        $origin = trim($data[0]); //no need to shift if we start the for at 1
        for ($i = $skip; $i < sizeof($data); $i++) {
          if ($data[$i] != '' && $data[$i] != 0) {

            $from_sids = $stop_dictionary[$origin];
            $to_sids = $stop_dictionary[trim($cols[$i])];

            if ($origin != $cols[$i]) {
            for ($j = 0; $j < count($from_sids); $j++) { //for each from id
              for ($k = 0; $k < count($to_sids); $k++) {
               $fare_rule = entity_get_controller('fare_rule')->create();

                $fare_rule->created_by = $user->name;
                $fare_rule->updated_by = $user->name;
                $fare_rule_opposite = clone $fare_rule;

                $fare_rule->origin_id = get_zone_for($from_sids[$j]);
                $fare_rule->destination_id = get_zone_for($to_sids[$k]);

                //create zone_ids for from_stop and to_stop if they do not exist
                if (!$fare_rule->origin_id) {
                  $fare_rule->origin_id = create_zone_for($from_sids[$j]);
                }

                if (!$fare_rule->destination_id) {
                  $fare_rule->destination_id = create_zone_for($to_sids[$k]);
                }

                if ($fare_rule->origin_id != $fare_rule->destination_id) {
                  $fare = get_fare(str_replace('$', '', $data[$i]));
                  $fare_rule->fare_id = $fare->fare_id;
                  fare_rule_save($fare_rule);

                  $fare_rule_opposite->destination_id = $fare_rule->origin_id;
                  $fare_rule_opposite->origin_id = $fare_rule->destination_id;
                  $fare_rule_opposite->fare_id = $fare->fare_id;

                  fare_rule_save($fare_rule_opposite);
                  unset($fare_rule);
                  unset($fare_rule_opposite);
                }
              }//end for $k
            }//end for $j
            }
          }// if $data
        } // for $i
        //increment skip to skip empty fare that exists at n - $i
        $skip++;
      }
    }
  }
  drupal_set_message(t('Fares have been uploaded'));
  $form_state['redirect'] = 'agency/' . $agency->aid . '/fares/matrix';
}

function gtfs_manager_agency_fares_download($agency) {
  $stop_names = fetch_stop_names(FALSE, $agency->aid);
  array_unshift($stop_names, ' ');


  $file = drupal_get_path('module', 'gtfs_manager') . '/files/' . str_replace(' ', '', preg_replace('/[^\w\d ]/ui', '', stripslashes($agency->agency_name))) . '_fares.csv';

  $fh = fopen($file, 'w');

  fputcsv($fh, $stop_names);

  array_shift($stop_names);

  foreach ($stop_names as $stop_name) {
    fputcsv($fh, array($stop_name));
  }

  if (file_exists($file)) {
    drupal_add_http_header('Content-Description', 'File Transfer');
    drupal_add_http_header('Content-Type', 'application/csv');
    drupal_add_http_header('Content-Disposition', 'attachment; filename='.basename($file));
    drupal_add_http_header('Content-Transfer-Encoding', 'binary');
    drupal_add_http_header('Expires', '0');
    drupal_add_http_header('Cache-Control', 'must-revalidate');
    drupal_add_http_header('Pragma', 'public');
    drupal_add_http_header('Content-Length', filesize($file));
    ob_clean();
    flush();
    readfile($file);
    fclose($fh);
  }

}

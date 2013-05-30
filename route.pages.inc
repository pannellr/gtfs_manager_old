<?php

/**
 * Form builder; create and display the form for a new route
 */
function gtfs_manager_route_form($form, &$form_state, $route = null, $agency = null) {

  //collect the agency id from wherever it is set
  $agency_id = isset($agency) ? $agency->aid : $route->aid;

  //set the id to identify this as the route edit form
  $form['#id'] = 'route-form';

  //save the route for later
  $form['#route'] = $route;
  $form_state['route'] = $route;

  //hidden field for acency id
  $form['gtfs_manager_aid'] = array(
    '#type' => 'hidden',
    '#value' => $agency_id,
  );
				    
  //Text field for route short name
  $form['gtfs_manager_route_short_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Route short name(example: Wolf2MoncAM)'),
    '#description' => t('An abstract identifier for the route'),
    '#default_value' => $route->route_short_name,
    '#size' => 40,
  );

  //Text field for route long name
  $form['gtfs_manager_route_long_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Route long name(example: Wolfvilled to Moncton)'),
    '#description' => t('A human readable identifier for the route'),
    '#default_value' => $route->route_long_name,
    '#size' => 60,
    '#required' => TRUE,
  );

  //Text field for route name
  $form['gtfs_manager_route_desc'] = array(
    '#type' => 'textarea',
    '#title' => t('Descriprion of route.'),
    '#description' => t('This would be a good place to include information not captured by this database.  Like stops the service is willing to make as an exception.'),
    '#default_value' => $route->route_desc,
    '#rows' => 12,
    '#cols' => 20,
  );

  //Textfield for route URL
  $form['gtfs_manager_route_url'] = array(
    '#type' => 'textfield',
    '#title' => t('URL for this route.'),
    '#description' => t('Th link for this specific route.'),
    '#default_value' => $route->route_url,
    '#size' => 80,
  );

  //Text field for route type !get from database!
  $form['gtfs_manager_transportation_type_id'] = array(
    '#type' => 'select',
    '#title' => t('Transportation Type'),
    '#description' => t('The transportation type for this route'),
    '#default_value' => $route->route_type_id,
    '#options' => fetch_transportation_types(),
    '#required' => TRUE,
  );

  // Add the buttons
  $form['buttons'] = array();
  $form['buttons']['#weight'] = 100;
  $form['buttons']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save Route'),
    '#weight' => 5,
    '#submit' => array('gtfs_manager_route_form_submit'),
  );
  
  if (!empty($route->rid)) {
    $form['buttons']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete'),
      '#weight' => 15,
      '#submit' => array('gtfs_manager_route_form_delete_submit'),
    );
  }

  $form['#validate'][] = 'gtfs_manager_route_form_validate';

  //allow other fields to be added
  field_attach_form('route', $route, $form, $form_state);

  return $form;
}

/**
 * Implements hook_validate() for route submissions
 */
function gtfs_manager_route_form_validate($form, &$form_state) {
  $route = $form_state['route'];

  //Field validations
}

/**
 * Implements hook_form_submit() for route
 */
function gtfs_manager_route_form_submit($form, &$form_state) {
  global $user;

  $route = $form_state['route'];
  // if this is a new record
  if (empty($route->created_by)) {
    $route->created_by = $user->name;
  }

  $route->updated_by = $user->name;

  $route->aid = $form_state['values']['gtfs_manager_aid'];
  $route->route_short_name = $form_state['values']['gtfs_manager_route_short_name'];
  $route->route_long_name = $form_state['values']['gtfs_manager_route_long_name'];
  $route->route_desc = $form_state['values']['gtfs_manager_route_desc'];
  $route->route_url = $form_state['values']['gtfs_manager_route_url'];
  $route->transportation_type_id = 
    $form_state['values']['gtfs_manager_transportation_type_id'];

  //notify field widgets
  field_attach_submit('route', $route, $form, $form_state);

  //Save
  route_save($route);

  drupal_set_message(t('Route saved'));

  $form_state['redirect'] = 'route/' . $route->rid;

}

function gtfs_manager_route_form_delete_submit($form, &$form_state) {
  $destination = array();
  if (isset($_GET['destination'])) {
    $destinaltion = drupal_get_destination();
    unset($_GET['destination']);
  }

  $route = $form['#route'];
  $form_state['redirect'] = array('route/' . $route->rid . '/delete', 
    array('query' => $destination));  
}

function route_page_edit($route) {
  drupal_set_title(t('<em>Edit @name route</em>', array('@name'=> $route->route_long_name)), PASS_THROUGH);
  return drupal_get_form('gtfs_manager_route_form', $route);
}

function route_delete_confirm($form, &$form_state, $route) {
  $form['#route'] = $route;
  $form['rid'] = array('#type' => 'value', '#value' => $route->rid);
  return confirm_form($form,
    t('Are you sure you want to delete the route: %name?', 
      array('%name' => $route->route_long_name)),
    'agency/' . $route->rid,
    t('This action cannot be undone.'),
    t('Delete'),
    t('Cancel')
  );
}

function route_delete_confirm_submit($form, &$form_state) {
  if ($form_state['values']['confirm']) {
    $route = route_load($form_state['values']['rid']);
    route_delete($form_state['values']['rid']);
  }

  watchdog('route', '@name deleted.', array('@name' => $route->route_long_name));

  drupal_set_message(t('The route %name has been deleted.', 
    array('%name' => $route->route_long_name)
  ));

  $form_state['redirect'] = 'agency/' . $route->aid . 'routes/view';
}


function gtfs_manager_load_route_detail($route) {
  $transportation_type = 
    transportation_type_load($route->transportation_type_id);
  $route->transportation_type_name = 
    $transportation_type->transportation_type_name;
  $variables['route'] = $route;
  return theme('gtfs_manager_route_detail', $variables);
}
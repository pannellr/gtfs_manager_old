<?php

/**
 * Form builder; create and display the form for a new route
 */
function gtfs_manager_route_form($form, &$form_state, $route = null, $agency = null) {

  //collect the agency id from wherever it is set
  $agency_id = isset($agency) ? $agency->aid : $route->aid;

  //set the id to identify this as the route edit form
  $form['#id'] = 'route-form';
  $form['#tree'] = TRUE;
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
    '#title' => t('Description of route.'),
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
    '#default_value' => $route->transportation_type_id,
    '#options' => fetch_transportation_types(),
    '#required' => TRUE,
  );

  $form['gtfs_manager_service_type'] = array(
    '#type' => 'radios',
    '#title' => 'Service Type',
    '#options' => array(0 => t('Fixed Route'), 1 => t('Service Area')),
    '#default_value' => 0,
  );

  $form['gtfs_manager_service_area_points'] = array(
    '#type' => 'textarea',
    '#field_suffix' => '<div id="polygon-canvas-container"></div>',
    '#attributes' => array(
      'class' => array('polygon-coordinates'),
    ),
    '#states' => array(
      'visible' => array(
        ':input[name="gtfs_manager_service_type"]' => array('value' => 1),
      )
    )
  );

  // Add the buttons
  $form['buttons'] = array();
  $form['buttons']['#weight'] = 100;
  $form['buttons']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save Route'),
    '#weight' => 5,
    '#attributes' => array(
      'class' => array('route-submit-button'),
     ),
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

  $path = drupal_get_path('module', 'gtfs_manager');

  $form['#attached'] = array('css' => array(
					    'type' => 'file',
					    'data' => $path
					    . '/route_form_theme.css',
					    ),
			     'js' => array(
					    'type' => 'file',
					    'data' => $path
					    . '/route_form.js',
					    ),

			     );

  $form['#validate'][] = 'gtfs_manager_route_form_validate';

  //allow other fields to be added
  field_attach_form('route', $route, $form, $form_state);

  return $form;
}

/**
 * Implements hook_validate() for route submissions
 */
function gtfs_manager_route_form_validate($form, &$form_state) {

}

/**
 * Implements hook_form_submit() for route
 */
function gtfs_manager_route_form_submit($form, &$form_state) {
  global $user;

  $route = $form_state['route'];
  $service_area = entity_get_controller('service_area')->create();

  // if this is a new record
  if (empty($route->created_by)) {
    $route->created_by = $user->name;
    $service_area->created_by = $user->name;
  }

  $route->updated_by = $user->name;
  $service_area->updated_by = $user->name;

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

  //create a routes_service_area_record
  $service_area->rid = $route->rid;
  service_area_save($service_area);

  //split the points from the form
  $points = explode(',', $form_state['values']['gtfs_manager_service_area_points']);
  //unpack
  foreach ($points as $point) {
    $point_values = explode('|', $point);
    //NEED VALIDATION SOMEWHERE--MUST BE AT LEAST 3 POINTS
    $service_area_point = entity_get_controller('service_area_point')->create();
    $service_area_point->created_by = $user->name;
    $service_area->updated_by_by = $user->name;
    $service_area_point->service_area_id = $service_area->service_area_id;

    foreach ($point_values as $key_val_string) {
      if (!empty($key_val_string)) {
	watchdog('key_val_string', $key_val_string);
	$key_val = explode(':', $key_val_string);
	$service_area_point->{$key_val[0]} = $key_val[1];
      }
    }
    //insert
    service_area_point_save($service_area_point);
  }

  //gtfs_manager_service_area_points

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

  drupal_set_message(t('The route %name has been deleted.',
    array('%name' => $route->route_long_name)
  ));

  $form_state['redirect'] = 'agency/' . $route->aid . '/routes/view';
}


function gtfs_manager_load_route_detail($route) {
  $transportation_type =
    transportation_type_load($route->transportation_type_id);
  $route->transportation_type_name =
    $transportation_type->transportation_type_name;
  $variables['route'] = $route;
  return theme('gtfs_manager_route_detail', $variables);
}

/**
 * Form builder; create and display the form for a new route service feature
 */
function gtfs_manager_route_feature_form($form, &$form_state, $route_service_feature, $agency, $route) {
  $route->service_feature_ids = fetch_features('route', $route->rid, TRUE);

  //Set the id to identify this as the route edit form
  $form['#id'] = 'route-service-feature-form';

  // save the route and service feature objects for later
  $form['#route'] = $route;
  $form_state['route'] = $route;
  $form['#agency'] = $agency;
  $form_state['agency'] = $agency;
  $form_state['route_service_feature'] = $route_service_feature;

  $form['create_link'] = array(
    '#markup' => l('Create a new feature', 'service_feature/new')
  );

  //checkboxes for all features
  $form['gtfs_manager_route_service_feature_ids'] = array(
    '#type' => 'checkboxes',
    '#options' => fetch_service_features(),
    '#title' => t('Select features provided by this route'),
    '#default_value' => $route->service_feature_ids,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save Route'),
    '#weight' => 5,
    '#submit' => array('gtfs_manager_route_service_features_form_submit'),
  );

  $form['#validate'][] = 'gtfs_manager_route_service_features_form_validate';

  return $form;
}

function gtfs_manager_route_service_features_form_validate($form, &$form_state) {

}

function gtfs_manager_route_service_features_form_submit($form, &$form_state) {
  global $user;
  $service_feature_ids = $form_state['route']->service_feature_ids;
  $route_service_feature = &$form_state['route_service_feature'];
  $agency = $form_state['agency'];

  // if this is a new record
  if (empty($route_service_feature->created_by)) {
    $route_service_feature->created_by = $user->name;
  }

  $route_service_feature->updated_by = $user->name;
  $route_service_feature->rid = $form_state['route']->rid;

  //make a copy so we can loop through and save the different features
  $copy = $route_service_feature;

  foreach ($form_state['values']['gtfs_manager_route_service_feature_ids'] as $key => $value) {
    if ($value != 0) {
      $copy->service_feature_id = $key;
      //Save
      route_service_feature_save($copy);
      unset($service_feature_ids[$key]);
    }
  }

  //delete unchecked features
  if (sizeof($service_feature_ids) != 0) {
      $query = 'select route_service_feature_id from route_service_features where rid = :rid and service_feature_id in (:sfids)';
      $result = db_query($query, array(':rid' => $route_service_feature->rid, ':sfids' => implode(',', $service_feature_ids)));
      while ($id = $result->fetchField()) {
        route_service_feature_delete(array($id));
      }
  }

  drupal_set_message(t('Route Service Feature(s) updated'));

  $form_state['redirect'] = 'agency/' . $agency->aid . '/routes';

}

/**
 * Save a new or updated route service feature
 *
 * @param $route_service_feature a populated route object
 */
function route_service_feature_save($route_service_feature) {
  return entity_get_controller('route_service_feature')->save($route_service_feature);
}

/**
 * deletes a route service featrue via controller
 * @param  $route_service_feature the service feature being deleted
 *
 */
function route_service_feature_delete($route_service_feature_id) {
  return entity_get_controller('route_service_feature')->delete($route_service_feature_id);
}

/*
 * returns features for a given route id
 */
function fetch_route_features($rid) {
  return fetch_features('route', $rid);
}


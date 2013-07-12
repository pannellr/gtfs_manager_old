<?php
/**
 * Form builder; create and display the form for a new agency
 */
function gtfs_manager_agency_form($form, &$form_state, $agency = null) {
  
  //Set the id to identify this as the agency edit form
  $form['#id'] = 'agency-form';

  // save the agency for later in case we need it
  $form['#agency'] = $agency;
  $form_state['agency'] = $agency;

  //Text field for agency name
  $form['gtfs_manager_agency_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Transporter name'),
    '#default_value' => $agency->agency_name,
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
  
  // Field Validation
  field_attach_form_validate('agency', $agency, $form, $form_state);
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
  $agency->agency_name = $form_state['values']['gtfs_manager_agency_name'];
  $agency->agency_url = $form_state['values']['gtfs_manager_agency_url'];
  $agency->agency_phone = $form_state['values']['gtfs_manager_agency_phone'];
  $agency->agency_email = $form_state['values']['gtfs_manager_agency_email'];
  $agency->agency_info = $form_state['values']['gtfs_manager_agency_info'];


  //notify field widgets
  field_attach_submit('agency', $agency, $form, $form_state);

  //Save
  agency_save($agency);

  drupal_set_message(t('Agency saved'));

  $form_state['redirect'] = 'agency/' . $agency->aid;

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

  drupal_set_message(t('The route %name has been deleted.', 
    array('%name' => $agency->agency_name)
  ));

  $form_state['redirect'] = 'agencies/view';
}

/**
 * List all agencies in database
 */
function gtfs_manager_all_agencies_view() {
  //select all agencies from database

  $agencies = entity_load('agency', FALSE);

  $page['title'] = t('Showing all agencies');
  $page['items'] = array();


  if (!empty($agencies)) {
    foreach ($agencies as $agency) {
      $link = l($agency->agency_name, 'agency/' . $agency->aid)
	. ' | ' 
	. l('View routes', 'agency/' . $agency->aid . '/routes');

	if (user_access('administer gtfs')) {
	  $link .= ' | ' 
	  . l('Add route', 'agency/' . $agency->aid . '/route/add')
	  . ' | ' 
	  . l('Add Feature', 'agency/' . $agency->aid . '/feature/add');
	}
      array_push($page['items'], $link);
    }
  }

  return theme('item_list', $page);
}

function agency_routes_page_view($agency, $view_mode = 'full') {

  $routes = route_load_multiple(FALSE, array('aid' => $agency->aid));
  
  $list['title'] = 'Showing routes for ' . $agency->agency_name;
  $list['items'] = array();

  foreach ($routes as $route) {
    $link = l($route->route_long_name, 'route/' . $route->rid);
    array_push($list['items'], $link);
  }
  return theme('item_list', $list);
}

function gtfs_manager_agency_detail($agency) {
  $variables['agency'] = $agency;
  return theme('gtfs_manager_agency_detail', $variables);
}

/**
 * Form builder; create and display the form for a new agency
 */
function gtfs_manager_agency_feature_form($form, &$form_state, $agency_service_feature, $agency) {
  
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
    //'#default_value' => $agency->service_feature_ids
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save Agency'),
    '#weight' => 5,
    '#submit' => array('gtfs_manager_agency_service_features_form_submit'),
  );
    
  $form['#validate'][] = 'gtfs_manager_agency_service_features_form_validate';

  return $form;
}

function fetch_service_features() {
  $result = db_select('service_features', 'sf')
    ->fields('sf', array('service_feature_id', 'service_feature_name'))
    ->execute();

  $service_features = array();
  
  while ($service_feature = $result->fetchObject()) {
    $service_features[$service_feature->service_feature_id] 
      = $service_feature->service_feature_name;
  }

  return $service_features;
}

function gtfs_manager_agency_service_features_form_validate($form, &$form_state) {

}

function gtfs_manager_agency_service_features_form_submit($form, &$form_state) {

}

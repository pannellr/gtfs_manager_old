<?php
/**
 * Form builder; create and display the form for a new agency
 */
function gtfs_manager_service_feature_form($form, &$form_state, $service_feature) {
  
  //Set the id to identify this as the agency edit form
  $form['#id'] = 'service-feature-form';

  // save the service_feature for later in case we need it
  $form['#service_feature'] = $service_feature;
  $form_state['service_feature'] = $service_feature;

  //Text field for service_feature name
  $form['gtfs_manager_service_feature_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Transporter name'),
    '#default_value' => $service_feature->service_feature_name,
    '#description' => t('The name of the transit service provider'),
    '#size' => 40,
    '#required' => TRUE,
  );

  $form['gtfs_manager_feature_icon_upload'] = array(
    '#title' => t('Upload icon image'),
    '#type'  => 'file',
  );

  //text area for service_feature information
  $form['gtfs_manager_service_feature_info'] = array(
    '#type' => 'textarea',
    '#title' => t('Service_Feature Information'),
    '#description' => t('This would be a good place to include any extra information that you would like to share with users.'),
    '#default_value' => $service_feature->service_feature_info,
    '#rows' => 12,
    '#cols' => 20,
  );

  // Add the buttons
  $form['buttons'] = array();
  $form['buttons']['#wieght'] = 100;
  $form['buttons']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save Service_Feature'),
    '#weight' => 5,
    '#submit' => array('gtfs_manager_service_feature_form_submit'),
  );

  //Add a delete button if user is editting
  if (!empty($service_feature->service_feature_id)) {
    $form['buttons']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete'),
      '#weight' => 15,
      '#submit' => array('gtfs_manager_service_feature_form_delete_submit'),
    );
  }
    
  $form['#validate'][] = 'gtfs_manager_service_feature_form_validate';

  //allow other fields to be added to form
  field_attach_form('service_feature', $service_feature, $form, $form_state);

  return $form;
}

function gtfs_manager_service_feature_form_validate($form, &$form_state) {

}

function gtfs_manager_service_feature_form_submit($form, &$form_state) {

}

function gtfs_manager_service_feature_form_delete_submit($form, &$form_state) {

}
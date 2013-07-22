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
    '#title' => t('Feature name'),
    '#default_value' => $service_feature->service_feature_name,
    '#description' => t('The name of the featur. ex. Wifi'),
    '#size' => 40,
    '#required' => TRUE,
  );

  $form['gtfs_manager_feature_icon_upload'] = array(
    '#title' => t('Upload icon image'),
    '#type'  => 'file',
    '#description' => t('Should be square. ex 100px X 100px'),
  );

  //text area for service_feature information
  $form['gtfs_manager_service_feature_info'] = array(
    '#type' => 'textarea',
    '#title' => t('Service Feature Information'),
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
    '#value' => t('Save Service Feature'),
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

  $file = file_save_upload('gtfs_manager_feature_icon_upload', array(
    'file_validate_extensions' => array('jpg jpeg png gif svg'),
    FALSE,
    FILE_EXISTS_RENAME
  ));


  if ($file) {
    if ($file = file_move($file, 'public://', FILE_EXISTS_RENAME)) {
      $form_state['values']['file'] = $file;
    } else {
      form_set_error('file', t('Failed to write the uploaded file to file folder'));
    }
  } else {
    form_set_error('file', t('No file was uploaded.'));
  }

}

function gtfs_manager_service_feature_form_submit($form, &$form_state) {
  global $user;

  $file = $form_state['values']['file'];
  unset($form_state['values']['file']);
  $file->status = FILE_STATUS_PERMANENT;
  file_save($file);


  $service_feature = &$form_state['service_feature'];

  // if this is a new record
  if (empty($agency->created_by)) {
    $service_feature->created_by = $user->name;
  }

  $service_feature->updated_by = $user->name;
  $service_feature->service_feature_name
    = trim($form_state['values']['gtfs_manager_service_feature_name']);

  $service_feature->service_feature_icon_url
    = file_create_url($file->uri);

  $service_feature->service_feature_info
    = addslashes(trim($form_state['values']['gtfs_manager_service_feature_info']));

  //Save
  service_feature_save($service_feature);

  drupal_set_message(t('Service feature saved'));

  $form_state['redirect'] = 'agencies/';


}

function gtfs_manager_service_feature_form_delete_submit($form, &$form_state) {

}

/**
 * Save a new or updated service feature
 *
 * @param $service_feature a populated agency object
 */
function service_feature_save($service_feature) {
  return entity_get_controller('service_feature')->save($service_feature);
}
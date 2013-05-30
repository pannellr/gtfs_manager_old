<?php

/**
 * Form builder; create and display the form for a new route
 */
function gtfs_manager_transportation_type_form($form, &$form_state, $transportation_type = null) {

  //set the id to identify this as the edit form
  $form['#id'] = 'transportation-type-form';

  //save transportation_type for later
  $form['#transportation_type'] = $transportation_type;
  $form_state['transportation_type'] = $transportation_type;

  //Text field for transportation type name
  $form['gtfs_manager_transportation_type_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Transportation Type Name'),
    '#default_value' => $transportation_type->transportation_type_name,
    '#description' => t('The name of the transportation type'),
    '#size' => 40,
    '#required' => TRUE,
  );

  // Add the buttons
  $form['buttons'] = array();
  $form['buttons']['#wieght'] = 100;
  $form['buttons']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save Transportation Type'),
    '#weight' => 5,
    '#submit' => array('gtfs_manager_transportation_type_form_submit'),
  );

  //Add a delete button if user is editting
  if (!empty($transportation_type->transportation_type_id)) {
    $form['buttons']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete'),
      '#weight' => 15,
      '#submit' => array('gtfs_manager_transportation_type_form_delete_submit'),
    );
  }
    
  $form['#validate'][] = 'gtfs_manager_transportation_type_form_validate';

  //allow other fields to be added to form
  field_attach_form('transportation_type', $transportation_type, $form, $form_state);


  return $form;
}

/**
 * Implements hook_validate() for route submissions
 */
function gtfs_manager_transportation_type_form_validate($form, &$form_state) {
  $transportation_type = $form_state['transportation_type'];
  field_attach_form_validate('transportation_type', $transportation_type, $form, 
    $form_state);
}

/**
 * Implements hook_form_submit() for route
 */
function gtfs_manager_transportation_type_form_submit($form, &$form_state) {
  global $user;

  $transportation_type = $form_state['transportation_type'];

  if (empty($transportation_type)) {
    $transportation_type->created_by = $user->name;
  }

  $transportation_type->updated_by = $user->name;
  $transportation_type->transportation_type_name = 
    $form_state['values']['gtfs_manager_transportation_type_name'];

  //notigy the fields
  field_attach_submit('transportation_type', $transportation_type, $form, 
    $form_state);
  
  //Save
  transportation_type_save($transportation_type);

  drupal_set_message(t('Transportation Type Saved'));

  $form_state['redirect'] = 'admin/config/gtfs_manager/transportation_types';
}

function gtfs_manager_transporation_type_form_delete_submit($form, &$form_state) {
  
}

function transporation_type_page_edit($transporation_type) {
  
}

function transportation_type_delete_confirm($form, &$form_state, $transportation_type) {
  
}

function transporation_type_delete_confirm_submit($form, &$form_state) {
  
}


function gtfs_manager_load_transportation_type_detail($transporation_type) {

}

function gtfs_manager_show_transportation_types(){

  $transportation_types = entity_load('transportation_type', FALSE);

  $page['title'] = t('Showing Transportation Types');
  $page['items'] = array();

  if (!empty($transportation_types)) {
    foreach ($transportation_types as $transportation_type){
      array_push($page['items'], $transportation_type->transportation_type_name);
    }
  }

  return theme('item_list', $page);

}


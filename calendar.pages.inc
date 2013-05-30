<?php
/**
 * Form builder; create and display the form for a new schedule
 */
function gtfs_manager_calendar_form($form, &$form_state, $calendar = null, $route= null) {
  //Set the id so that drupal knows this is the calendar edit form
  $form['#id'] = 'calendar-form';

  // hang on to the calendar object in case we need it later
  $form['#calendar'] = $calendar;
  $form_state['calendar'] = $calendar; 

  $form_state['route'] = $route;
  
  //Date Field for Schedule Start Date
  $form['gtfs_manager_schedule_start_date'] = array(
    '#type' => 'date',
    '#title' => t('Schedule Start Date'),
    '#description' => t('The start date for this route'),
    '#default_value' => $calendar->start_date,
    '#required' => TRUE,
  );

  //Date field for schedule end date
  $form['gtfs_manager_schedule_end_date'] = array(
    '#type' => 'date',
    '#title' => t('Schedule End Date'),
    '#description' => t('The end date for this route'),
    '#default_value' => $calendar->end_date,
    '#required' => TRUE,
  );

  //checkboxes for days of the week
  $form['gtfs_manager_schedule_days'] = array(
    '#type' => 'checkboxes',
    '#options' => drupal_map_assoc(array(
			t('monday'), 
			t('tuesday'), 
			t('wednesday'),
			t('thursday'),
			t('friday'),
			t('saturday'),
			t('sunday')
					 )),
    '#title' => t('Which days will this route run?FIXME DEFAULTS'),
  );

  // Add buttons
  $form['buttons'] = array();
  $form['buttons']['#weight'] = 100;
  $form['buttons']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Create Calendar'),
    '#weight' => 5,
    '#submit' => array('gtfs_manager_calendar_form_submit'),
  );

  // Add a delete button for edit forms
  if (!empty($calendar->service_id)) {
    $form['buttons']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete'),
      '#weight' => 15,
      '#submit' => array('gtfs_manager_calendar_form_delete_submit'),
    );
  }
  
  return $form;
}

/**
 * Implements hook_validate() for calendar submissions
 */
function gtfs_manager_calendar_form_validate($form, &$form_state) {
  $calendar = $form_state['calendar'];
  
  // Field Validation
  field_attach_form_validate('calendar', $calendar, $form, $form_state);
}

/**
 * Submit handler for schedule
 */
function gtfs_manager_calendar_form_submit($form, &$form_state) {
  global $user;

  $calendar = $form_state['calendar'];

  if (empty($calendar->created_by)) {
    $calendar->created_by = $user->name;
  }

  $calendar->updated_by = $user->name;

  //this is ugly should come through the form 1 or 0
  $calendar->monday = 
    isset($form_state['values']['gtfs_manager_schedule_days']['monday']) ?
    1 : 0;
  $calendar->tuesday = 
    isset($form_state['values']['gtfs_manager_schedule_days']['tuesday']) ?
    1 : 0;
  $calendar->wednesday = 
    isset($form_state['values']['gtfs_manager_schedule_days']['wednesday']) ?
    1 : 0;
  $calendar->thursday = 
    isset($form_state['values']['gtfs_manager_schedule_days']['thursday']) ?
    1 : 0;
  $calendar->friday = 
    isset($form_state['values']['gtfs_manager_schedule_days']['friday']) ?
    1 : 0;
  $calendar->saturday = 
    isset($form_state['values']['gtfs_manager_schedule_days']['saturday']) ?
    1 : 0;
  $calendar->sunday = 
    isset($form_state['values']['gtfs_manager_schedule_days']['sunday']) ?
    1 : 0;

   $start_date_string =
   $form_state['values']['gtfs_manager_schedule_start_date']['year'] 
    . '-'
    . $form_state['values']['gtfs_manager_schedule_start_date']['month']
    . '-'
    . $form_state['values']['gtfs_manager_schedule_start_date']['day'];
  
  $end_date_string = 
    $form_state['values']['gtfs_manager_schedule_end_date']['year'] 
    . '-'
    . $form_state['values']['gtfs_manager_schedule_end_date']['month']
    . '-'
    . $form_state['values']['gtfs_manager_schedule_end_date']['day'];

  $calendar->start_date = strtotime($start_date_string);
  $calendar->end_date = strtotime($end_date_string);

  // notify field widgets
  field_attach_submit('calendar', $calendar, $form, $form_sate);

  calendar_save($calendar);

  drupal_set_message(t('Calendar Saved'));
  
  $form_state['redirect'] = 'route/' . $form_state['route']->rid 
    . '/stop/add';
}

function gtfs_manager_calendar_form_delete_submit($form, &$form_state) {
  $destination = array();
  if (isset($_GET['destination'])) {
    $destinaltion = drupal_get_destination();
    unset($_GET['destination']);
  }

  $calendar = $form['#calendar'];
  $form_state['redirect'] = array(
    'calendar/' . $calendar->service_id . '/delete', 
    array('query' => $destination)
  );  
}


function calendar_page_edit($calendar) {
  drupal_set_title(t('<em>Edit Calendar @from to @to</em>', 
		     array('@from'=> $calendar->start_date, 
			   '@to' => $calendar->end_date,
		     )
  ), PASS_THROUGH);
  return drupal_get_form('gtfs_manager_calendar_form', $calendar);
}

function calendar_delete_confirm($form, &$form_state, $calendar) {
  $form['#calendar'] = $calendar;
  $form['service_id'] = 
    array('#type' => 'value', '#value' => $calendar->service_id);
  
  return confirm_form($form,
    t('Are you sure you want to delete this calendar?'),
    'calendar/' . $calendar->service_id,
    t('This action cannot be undone.'),
    t('Delete'),
    t('Cancel')
  );
}

function calendar_delete_confirm_submit($form, &$form_state) {
  if ($form_state['values']['confirm']) {
    $calendar = calendar_load($form_state['values']['service_id']);
    calendar_delete($form_state['values']['service_id']);
  }

  watchdog('calendar', '@id deleted.', array('@name' => $calendar->service_id));

  drupal_set_message(t('Calendar deleted.'));

  $form_state['redirect'] = 'calendars/';
}

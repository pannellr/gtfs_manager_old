<?php
/**
 * Form builder for new trip, stop, and stop_time
 */
function gtfs_manager_stop_form($form, &$form_state, $stop = null, $route = null) {
  
  //collect the route id
  $route_id = isset($route) ? $route->rid : $stop->rid;

  //Set the id ot identify this as the stop edit form
  $form['#id'] = 'stop-form';

  //save the stop for later
  $form['#stop'] = $stop;
  $form_state['stop'] = $stop;

  //hidden field for route id
  $form['gtfs_manager_rid'] = array(
    '#type' => 'hidden',
    '#value' => $route_id,
  );

  //Stop Section

  //Textfield for stop name
  $form['gtfs_manager_stop_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Name for this stop. example: Truro'),
    '#description' => t('The name of this stop'),
    '#size' => 40,
    '#default_value' => $stop->stop_name,
    '#required' => TRUE,
  );

  // Text area for stop description
  $form['gtfs_manager_stop_desc'] = array(
    '#type' => 'textarea',
    '#title' => t('Stop description'),
    '#description' => t('Add details specific to this stop here.'),
    '#cols' => 40,
    '#rows' => 6,
    '#default_value' => $stop->stop_desc,
  );

  //tetfield for stop lattitude
  $form['gtfs_manager_stop_northing'] = array(
    '#type' => 'textfield',
    '#title' => t('UTM Northing for this this stop location'),
    '#description' => t('UTM Northing for this stop'),
    '#size' => 24,
    '#default_value' => $stop->stop_northing,
    '#required' => TRUE,
  );  

  //tetfield for stop longitude
  $form['gtfs_manager_stop_easting'] = array(
    '#type' => 'textfield',
    '#title' => t('UTM Easting for this this stop location'),
    '#description' => t('UTM Easting for this stop'),
    '#size' => 24,
    '#default_value' => $stop->stop_easting,
    '#required' => TRUE,
  ); 

//tetfield for stop lattitude
  $form['gtfs_manager_utm_zone'] = array(
    '#type' => 'textfield',
    '#title' => t('UTM zone for this stop location'),
    '#description' => t('UTM Northing zone for this stop'),
    '#size' => 4,
    '#default_value' => $stop->utm_zone,
    '#required' => TRUE,
  );

  //checkbox for wheelchair access
  $form['gtfs_manager_wheelchair_boarding'] = array(
    '#type' => 'checkbox',
    '#title' => t('Has wheelchair access'),
    '#default_value' => $stop->wheelchair_boarding,
  );

  //timepicker for stop arrival time
  $form['gtfs_manager_stop_arrival_time'] = array(
    '#type' => 'textfield',
    '#title' => t('Arrival Time for this stop'),
    '#description' => t('The arrival time'),
    '#attributes' => array('class' => array('form-time')),
    '#default_value' => $stop->arrival_time,
    '#size' => 20,
    '#required' => TRUE,
  );

  //timepicker for stop arrival time
  $form['gtfs_manager_stop_departure_time'] = array(
    '#type' => 'textfield',
    '#title' => t('Departure Time for this stop'),
    '#description' => t('The departure time'),
    '#attributes' => array('class' => array('form-time')),
    '#size' =>20,
    '#default_value' => $stop->departure_time,
    '#required' => TRUE,
  );

  //tetfield for stop sequence number
  $form['gtfs_manager_stop_sequence'] = array(
    '#type' => 'textfield',
    '#title' => t('Sequence number of this stop'),
    '#description' => t('The stop sequence number'),
    '#size' => 3,
    '#default_value' => $stop->stop_sequence,
    '#required' => TRUE,
  );
  
  //start date popup
  $form['gtfs_manager_schedule_start_date'] = array(
    '#type' => 'date_popup',
    '#date_format' => 'm/d/Y',
    '#default_value' => $stop->start_date,
    '#date_year_range' => '0:+2',
    '#date_label_position' => 'invisible',
    '#title' => t('Schedule end date'),
    '#required' => TRUE,
  );
    //Date Field for Schedule Start Date
  /*
  $form['gtfs_manager_schedule_start_date'] = array(
    '#type' => 'date',
    '#title' => t('Schedule start date'),
    '#description' => t('The start date for this route'),
    '#default_value' => array(
      'month' => date('m', $stop->start_date),
      'day' => date('d', $stop->start_date),
      'year' => date('Y', $stop->start_date),
    ),
    '#required' => TRUE,
  );
  */

  //end date popup
  $form['gtfs_manager_schedule_end_date'] = array(
    '#type' => 'date_popup',
    '#date_format' => 'm/d/Y',
    '#default_value' => $stop->end_date,
    '#date_year_range' => '0:+2',
    '#date_label_position' => 'invisible',
    '#title' => t('Schedule end date'),
    '#required' => TRUE,
  );

  //Date field for schedule end date
  /*
  $form['gtfs_manager_schedule_end_date'] = array(
    '#type' => 'date',
    '#title' => t('Schedule End Date'),
    '#description' => t('The end date for this route'),
    '#default_value' => array(
      'month' => date('m', $stop->end_date),
      'day' => date('d', $stop->end_date),
      'year' => date('Y', $stop->end_date),
    ),
    '#required' => TRUE,
  );
  */

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
    '#default_value' => $stop->days,
    '#title' => t('Which days will this route run?'),
  );


  //Add buttons
  $form['buttons'] = array();
  $form['buttons']['#weight'] = 100;
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save stop'),
    '#weight' => 5,
    '#submit' => array('gtfs_manager_stop_form_submit'),
  );
  
  // display delete button in edit form
  if (!empty($stop->sid)) {
    $form['buttons']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete'),
      '#weight' => 15,
      '#submit' => array('gtfs_manager_stop_form_delete_submit'),
    );
  }

  $form['#validate'][] = 'gtfs_manager_stop_form_validate';

  //allow other fields to be added
  field_attach_form('stop', $stop, $form, $form_state);

  return $form;
}

/**
 * Imiplements hook_validate() for stop submissions
 */
function gtfs_manager_stop_form_validate($form, &$form_state) {
  $stop = $form_state['stop'];
  // field validations if any
  field_attach_form_validate('stop', $stop, $form, $form_state);
}

function gtfs_manager_stop_form_submit($form, &$form_state) {
  global $user;

  $stop = $form_state['stop'];

  $trip_id = empty($stop->tid) ? '' : $stop->tid;
  $service_id = empty($stop->service_id) ? '' : $stop->service_id;
  //create trip and stop time objects
  $trip = entity_get_controller('trip')->create($trip_id);
  $stop_time = entity_get_controller('stop_time')->create($trip_id);
  $calendar = entity_get_controller('calendar')->create($service_id);

  // if it is a new record set created_by
  if (empty($stop->created_by)) {
    $stop->created_by = $user->name;
    $trip->created_by = $user->name;
    $stop_time->created_by = $user->name;
    $calendar->created_by = $user->name;
  }

  $stop->updated_by = $user->name;
  $trip->updated_by = $user->name;
  $stop_time->updated_by = $user->name;
  $calendar->updated_by = $user->name;  

  watchdog('form_state', $form_state['values']['gtfs_manager_schedule_days']['monday']);
  //Calendar Save
  $calendar->monday = 
    $form_state['values']['gtfs_manager_schedule_days']['monday'] ? 1 : 0;
  $calendar->tuesday = 
    $form_state['values']['gtfs_manager_schedule_days']['tuesday'] ? 1 : 0;
  $calendar->wednesday = 
    $form_state['values']['gtfs_manager_schedule_days']['wednesday'] ? 1 : 0;
  $calendar->thursday = 
    $form_state['values']['gtfs_manager_schedule_days']['thursday'] ? 1 : 0;
  $calendar->friday = 
    $form_state['values']['gtfs_manager_schedule_days']['friday'] ? 1 : 0;
  $calendar->saturday = 
    $form_state['values']['gtfs_manager_schedule_days']['saturday'] ? 1 : 0;
  $calendar->sunday = 
    $form_state['values']['gtfs_manager_schedule_days']['sunday'] ? 1 : 0;

  $calendar->start_date = 
    strtotime($form_state['values']['gtfs_manager_schedule_start_date']);
  $calendar->end_date = 
    strtotime($form_state['values']['gtfs_manager_schedule_end_date']);

  // notify field widgets
  field_attach_submit('calendar', $calendar, $form, $form_sate);

  watchdog('calendar_save', $calendar->start_date);
  calendar_save($calendar);


  // Trip Save
  $trip->rid = $form_state['values']['gtfs_manager_rid'];
  $trip->service_id = $calendar->service_id;
  
  field_attach_submit('trip', $trip, $form, $form_state);

  trip_save($trip);

  // Save stop
  $stop->stop_name = $form_state['values']['gtfs_manager_stop_name'];
  $stop->stop_desc = $form_state['values']['gtfs_manager_stop_desc'];
  $stop->stop_northing = $form_state['values']['gtfs_manager_stop_northing'];
  $stop->stop_easting = $form_state['values']['gtfs_manager_stop_easting'];
  $stop->utm_zone = $form_state['values']['gtfs_manager_utm_zone'];
  $stop->wheelchair_boarding = 
    $form_state['values']['gtfs_manager_wheelchair_boarding'];

  field_attach_submit('stop', $stop, $form, $form_state);

  stop_save($stop);

  // stop time save
  $stop_time->tid = $trip->tid;
  $stop_time->sid = $stop->sid;
  $stop_time->arrival_time = 
    $form_state['values']['gtfs_manager_stop_arrival_time'];
  $stop_time->departure_time = 
    $form_state['values']['gtfs_manager_stop_departure_time'];
  $stop_time->stop_sequence = 
    $form_state['values']['gtfs_manager_stop_sequence'];

  field_attach_submit('stop_time', $stop_time, $form, $form_state);

  stop_time_save($stop_time);

  drupal_set_message(t('Your stop has been submitted'));  

  $form_state['redirect'] = 'route/' . $trip->rid . '/stops';
}

function gtfs_manager_stop_form_delete_submit($form, &$form_state) {
  $destination = array();
  if (isset($_GET['destination'])) {
    $destinaltion = drupal_get_destination();
    unset($_GET['destination']);
  }

  $stop = $form['#stop'];
  $form_state['redirect'] = array('stop/' . $stop->sid . '/delete', 
    array('query' => $destination));  
}

function stop_page_edit($stop) {
  drupal_set_title(t('<em>Edit stop @name</em>', array('@name'=> $stop->stop_name)), PASS_THROUGH);
  return drupal_get_form('gtfs_manager_stop_form', $stop);
}

function stop_delete_confirm($form, &$form_state, $stop) {
  $form['#stop'] = $stop;
  $form['sid'] = array('#type' => 'value', '#value' => $stop->sid);
  return confirm_form($form,
    t('Are you sure you want to delete the stop: %name?', 
      array('%name' => $stop->stop_name)),
    'agency/' . $stop->sid,
    t('This action cannot be undone.'),
    t('Delete'),
    t('Cancel')
  );
}

function stop_delete_confirm_submit($form, &$form_state) {
  if ($form_state['values']['confirm']) {
    $stop = stop_load($form_state['values']['sid']);
    stop_delete($form_state['values']['sid']);
    trip_delete($stop->tid);
    stop_time_delete($stop->tid, $stop->sid);
  }

  watchdog('stop', 'Stop at @name deleted.', array('@name' => $stop->stop_name));

  drupal_set_message(t('The stop at %name has been deleted.', 
    array('%name' => $stop->stop_name)
  ));

  $form_state['redirect'] = 'route/' . $stop->rid . '/stops';
}


function gtfs_manager_load_stop_detail($stop) {
  $variables['stop'] = $stop;
  return theme('gtfs_manager_stop_detail', $variables);
}

function gtfs_manager_route_stops_view($route) {
  $variables['route'] = $route;
  //populate stops
  $stops = stop_load_multiple(array(), array('rid', $route->rid, '='));
  
  //hmmmmmmm should we theme the stops first??
  $variables['stops'] = $stops;
  return theme('gtfs_manager_route_stops_view', $variables);
}
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

  $form['gtfs_manager_province_id'] = array(
    '#type' => 'select',
    '#title' => t('Province/State'),
    '#default_value' => $stop->province_id,
    '#options' => fetch_provinces(),
    '#required' => TRUE,
  );

  $form['gtfs_manager_time_zone_id'] = array(
    '#type' => 'select',
    '#title' => t('Time zone'),
    '#default_value' => $stop->time_zone_id,
    '#options' => fetch_time_zones(),
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

    //checkbox for wheelchair access
  $form['gtfs_manager_wheelchair_boarding'] = array(
    '#type' => 'checkbox',
    '#title' => t('Has wheelchair access'),
    '#default_value' => $stop->wheelchair_boarding,
  );

  //tetfield for stop lattitude
  $form['gtfs_manager_stop_lat'] = array(
    '#type' => 'textfield',
    '#title' => t('Latitude for this this stop location'),
    '#description' => t('Lattitude for this stop.  Up to 8 points of precision.'),
    '#size' => 24,
    '#default_value' => $stop->stop_lat,
    '#required' => TRUE,
  );

  //tetfield for stop longitude
  $form['gtfs_manager_stop_long'] = array(
    '#type' => 'textfield',
    '#title' => t('Longitude for this this stop location'),
    '#description' => t('Longintude for this stop. Up to 8 points of precision.'),
    '#size' => 24,
    '#default_value' => $stop->stop_long,
    '#required' => TRUE,
  );

  //tetfield for trip distance
  $form['gtfs_manager_shape_dist_traveled'] = array(
    '#type' => 'textfield',
    '#title' => t('Distance from previous stop in kilometers'),
    '#description' => t('Up to 3 decimals of accuracy.'),
    '#size' => 24,
    '#default_value' => $stop->shape_dist_traveled,
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
    '#type' => 'select',
    '#title' => t('Sequence'),
    '#options' => fetch_sequence_select($route_id),
    '#default_value' => ($stop->stop_sequence - 1),
    '#required' => TRUE,
  );

  //start date popup
  $form['gtfs_manager_schedule_start_date'] = array(
    '#type' => 'date_popup',
    '#date_format' => 'd-m-Y',
    '#default_value' => date('Y-m-d H:i:s', $stop->start_date),
    '#date_year_range' => '0:+2',
    '#date_label_position' => 'invisible',
    '#title' => t('Schedule start date'),
    '#required' => TRUE,
  );

  //end date popup
  $form['gtfs_manager_schedule_end_date'] = array(
    '#type' => 'date_popup',
    '#date_format' => 'd-m-Y',
    '#default_value' => date('Y-m-d H:i:s', $stop->end_date),
    '#date_year_range' => '0:+2',
    '#date_label_position' => 'invisible',
    '#title' => t('Schedule end date'),
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

  $is_new = empty($stop->sid);

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

  //Calendar Save
  $calendar->monday =
    !empty($form_state['values']['gtfs_manager_schedule_days']['monday'])
    ? 1 : 0;
  $calendar->tuesday =
    !empty($form_state['values']['gtfs_manager_schedule_days']['tuesday'])
    ? 1 : 0;
  $calendar->wednesday =
    !empty($form_state['values']['gtfs_manager_schedule_days']['wednesday'])
    ? 1 : 0;
  $calendar->thursday =
    !empty($form_state['values']['gtfs_manager_schedule_days']['thursday'])
    ? 1 : 0;
  $calendar->friday =
    !empty($form_state['values']['gtfs_manager_schedule_days']['friday'])
    ? 1 : 0;
  $calendar->saturday =
    !empty($form_state['values']['gtfs_manager_schedule_days']['saturday'])
    ? 1 : 0;
  $calendar->sunday =
    !empty($form_state['values']['gtfs_manager_schedule_days']['sunday'])
    ? 1 : 0;

  $calendar->start_date =
    strtotime($form_state['values']['gtfs_manager_schedule_start_date']);
  $calendar->end_date =
    strtotime($form_state['values']['gtfs_manager_schedule_end_date']);

  // notify field widgets
  field_attach_submit('calendar', $calendar, $form, $form_sate);

  calendar_save($calendar);


  // Trip Save
  $trip->rid = $form_state['values']['gtfs_manager_rid'];
  $trip->service_id = $calendar->service_id;

  field_attach_submit('trip', $trip, $form, $form_state);

  trip_save($trip);

  // Save stop
  $stop->stop_name = trim($form_state['values']['gtfs_manager_stop_name']);
  $stop->stop_desc = trim($form_state['values']['gtfs_manager_stop_desc']);
  $stop->stop_lat = trim($form_state['values']['gtfs_manager_stop_lat']);
  $stop->stop_long = trim($form_state['values']['gtfs_manager_stop_long']);

  $stop->province_id = $form_state['values']['gtfs_manager_province_id'];
  $stop->time_zone_id = $form_state['values']['gtfs_manager_time_zone_id'];

  field_attach_submit('stop', $stop, $form, $form_state);

  stop_save($stop);


  if ($form_state['values']['gtfs_manager_wheelchair_boarding'] == 1) {
    $feature = entity_get_controller('stop_service_feature')->create();

    //get wheelchair access feature id
    $feature->sid = $stop->sid;
    $feature->service_feature_id = db_query("select service_feature_id from service_features where service_feature_name = 'Wheelchair Access'")->fetchField();
    $feature->created_by = $user->name;
    $feature->updated_by = $user->name;
    entity_get_controller('stop_service_feature')->save($feature);
  }

  // stop time save
  $stop_time->tid = $trip->tid;
  $stop_time->sid = $stop->sid;
  $stop_time->shape_dist_traveled = trim($form_state['values']['gtfs_manager_shape_dist_traveled']);
  watchdog('stop time', serialize($stop_time));
  $stop_time->arrival_time = $form_state['values']['gtfs_manager_stop_arrival_time'];
  $stop_time->departure_time = $form_state['values']['gtfs_manager_stop_departure_time'];

  //maintain sequence by reordering the sequence with each addition
  $update_type = $is_new ? 'new' : 'edit';

  $stop_time->stop_sequence = update_route_sequence(
    $trip->rid,
    $form_state['values']['gtfs_manager_stop_sequence'],
    $update_type,
    $stop->sid
  );

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
    stop_delete($stop);
    update_route_sequence($stop->rid, $stop->stop_sequence, 'delete');
  }

  watchdog('stop', 'Stop at @name deleted.', array('@name' => $stop->stop_name));

  drupal_set_message(t('The stop at %name has been deleted.',
    array('%name' => $stop->stop_name)
  ));

  $form_state['redirect'] = 'route/' . $stop->rid . '/stops';
}


//function gtfs_manager_load_stop_detail($stop) {
//  $variables['stop'] = $stop;
//  return theme('gtfs_manager_stop_detail', $variables);
//}

function gtfs_manager_route_stops_view($route) {
  $variables['route'] = $route;
  //populate stops
  $stops = stop_load_multiple(array(), array('rid', $route->rid, '='));

  if (sizeof($stops > 0)) {
    $variables['stops'] = $stops;
    return theme('gtfs_manager_route_stops_view', $variables);
  } else {
    drupal_set_message('There are not stops for this route.');
  }
}

function fetch_provinces() {
  $options = array();

  $provinces = entity_load('province', FALSE);

  if ($provinces) {
    foreach ($provinces as $province) {
      $options[$province->province_id] = t($province->name);
    }
  }
  return $options;
}



function fetch_sequence_select($rid) {
  $options = array();

  $query =<<<SQL
  select s.stop_name
    ,  st.stop_sequence
  from stops s
  join stop_times st
    on st.sid = s.sid
  join trips t
    on t.tid = st.tid
    where t.rid = :rid
  order by st.stop_sequence
SQL;

  $results = db_query($query, array(':rid' => $rid));

  $options[0] = 'First';

  while ($stop = $results->fetchObject()) {
    $options[$stop->stop_sequence] = 'After ' . $stop->stop_name;
  }

  array_push($options, 'Last');

  return $options;
}

function update_route_sequence($rid, $sequence, $update_type, $sid = null) {

  // for delete and update sequence is current position, but for insert
  // it is the position before the new record
  $new_record_sequence = $sequence + 1;

  if ($update_type == 'delete') {

      $query = <<<SQL
        update stop_times
          set stop_sequence = stop_sequence - 1
        where tid in (
          select tid
	  from trips
          where rid = :rid)
        and stop_sequence > :sq
SQL;

    db_query($query, array(':rid' => $rid, ':sq' => $new_record_sequence));

  } elseif ($update_type == 'new') {

    //check to see if the given sequence number already exists
    $query =<<<SQL
      select 1
      from stop_times st
      join trips t
        on t.tid = st.tid
      where t.rid = :rid
      and st.stop_sequence = :sq
SQL;

    $exists =
      db_query($query, array(':rid' => $rid, ':sq' => $new_record_sequence))
      ->fetchField();

    if ($exists || ($sequence == 0)) {

      $query = <<<SQL
        update stop_times
          set stop_sequence = stop_sequence + 1
        where tid in (
          select tid
	  from trips
          where rid = :rid)
        and stop_sequence > :sq
SQL;

      db_query($query, array(':rid' => $rid, ':sq' => $new_record_sequence));
    } else {
      // if the record will go last in the sequence then we don't need
      // to increment new_record_sequence
      $new_record_sequence = $new_record_sequence - 1;
    }

  } elseif ($update_type = 'edit') {
    $current_position = fetch_stop_position($sid);

    //move down the list
    if ($current_position > $sequence) {
      $query = <<<SQL
        update stop_times
          set stop_sequence = stop_sequence + 1
        where tid in (
          select tid
	  from trips
          where rid = :rid)
        and stop_sequence >= :new
        and stop_sequence <  :cur
SQL;

      db_query($query,
        array(':rid' => $rid,
	      ':new' => $new_record_sequence,
	      ':cur' => $current_position
	)
      );

    //move up the list
    } elseif ($current_position < $sequence) {
      $new_record_sequence = $new_record_sequence - 1;

      $query = <<<SQL
        update stop_times
          set stop_sequence = stop_sequence - 1
        where tid in (
          select tid
	  from trips
          where rid = :rid)
        and stop_sequence <= :new
        and stop_sequence > :cur
SQL;

      db_query($query, array(':rid' => $rid,
			     ':new' => $new_record_sequence,
			     ':cur' => $current_position
			     )
	       );

    }
  }

  return $new_record_sequence;
}

function fetch_stop_position($sid) {
  $current = db_select('stop_times', 'st')
    ->fields('st', array('stop_sequence'))
    ->condition('sid', $sid, '=')
    ->execute()
    ->fetchField();

  return $current;
}

/**
 * Form builder for stops upload
 */
function gtfs_manager_route_stops_upload_form($form, &$form_state, $route) {

  $form_state['route'] = $route;

  $form['gtfs_manager_stops_file_upload'] = array(
    '#title' => t('Upload file'),
    '#type'  => 'file',
  );

  $form['submit'] = array(
    '#type'  =>  'submit',
    '#value'  =>  'Submit'
  );

  return $form;

}

function gtfs_manager_route_stops_upload_form_validate($form, &$form_state) {
  $route = $form_state['route'];

  $file = file_save_upload('gtfs_manager_stops_file_upload', array(
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

function gtfs_manager_route_stops_upload_form_submit($form, &$form_state) {
  $file = $form_state['values']['file'];
  unset($form_state['values']['file']);
  $file->status = FILE_STATUS_PERMANENT;
  file_save($file);

  //load the file into the database
  if (($fh = fopen(file_create_url($file->uri), 'r')) != FALSE) {
    $cols = array();
    while (($data = fgetcsv($fh, 1000, ",")) != FALSE) {
      //grab the columns on the first iteration
      if (empty($cols)) {
        foreach ($data as $index => $col) {
          $cols[] = $col;
        }
        //load the data
      } else {

        $days_of_week = array(
          'monday',
          'tuesday',
          'wednesday',
          'thursday',
          'friday',
          'saturday',
          'sunday',
        );

        for ($i = 0; $i < count($data); $i++) {
          if (in_array($cols[$i], $days_of_week)) {
            $form_state['values']['gtfs_manager_schedule_days'][$cols[$i]] = trim($data[$i]);
          } else {
            if ($cols[$i] == 'gtfs_manager_stop_desc') {
              $data[$i] = addslashes($data[$i]);
            }
            $form_state['values'][$cols[$i]] = trim($data[$i]);
          }
        }

        $route = $form_state['route'];
        $form_state['values']['gtfs_manager_rid'] = $route->rid;
        $stop = entity_get_controller('stop')->create();
        $form['#stop'] = $stop;
        $form_state['stop'] = $stop;
        gtfs_manager_stop_form_submit($form, $form_state, $stop, $route);
      }
    }
  }
  drupal_set_message(t('Your stops have been uploaded'));
}

function gtfs_manager_stop_calendar_date_form($form, &$form_state, $service_id, $calendar_date, $rid) {


  //Set the id ot identify this as the stop edit form
  $form['#id'] = 'stop-calendar-date-form';

  //save the stop for later
  $form['#service_id'] = $service_id;
  $form_state['service_id'] = $service_id;
  $form_state['calendar_date'] = $calendar_date;
  $form_state['rid'] = $rid;

  //start date popup
  $form['gtfs_manager_calendar_date'] = array(
    '#type' => 'date_popup',
    '#date_format' => 'd-m-Y',
    '#default_value' => date('Y-m-d H:i:s', time()),
    '#date_year_range' => '0:+2',
    '#date_label_position' => 'invisible',
    '#title' => t('Exception Date'),
    '#required' => TRUE,
  );

  $form['gtfs_manager_exception_type'] = array(
    '#type' => 'select',
    '#title' => t('Exception Type'),
    '#default_value' => 0,
    '#options' => array( '' => t('Choose exception type'),
			 1 => t('Add service for this date'),
			 2 => t('Remove service for this date')
			 ),
    '#required' => TRUE,
  );

  //Add buttons
  $form['buttons'] = array();
  $form['buttons']['#weight'] = 100;
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save Exception'),
    '#weight' => 5,
    '#submit' => array('gtfs_manager_stop_calendar_date_form_submit'),
  );

  // display delete button in edit form
  if (!empty($service_id)) {
    $form['buttons']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete'),
      '#weight' => 15,
      '#submit' => array('gtfs_manager_stop_calendar_date_form_delete_submit'),
    );
  }

  $form['#validate'][] = 'gtfs_manager_stop_calendar_date_form_validate';

  return $form;
}

function gtfs_manager_stop_calendar_date_form_validate($form, &$form_state) {
  if($form_state['values']['gtfs_manager_exception_type'] == '') {
    form_set_error('gtfs_manager_exception_type', 'Please choose and exception type');
  }
}

function gtfs_manager_stop_calendar_date_form_submit($form, &$form_state) {

  global $user;

  $calendar_date = $form_state['calendar_date'];


  // if it is a new record set created_by
  if (empty($stop->created_by)) {
    $calendar_date->created_by = $user->name;
  }

  $calendar_date->updated_by = $user->name;

  $calendar_date->service_id = $form_state['service_id'];

  $calendar_date->exception_date
    = strtotime($form_state['values']['gtfs_manager_calendar_date']);

  $calendar_date->exception_type
    = $form_state['values']['gtfs_manager_exception_type'];

  calendar_date_save($calendar_date);

  drupal_set_message('Exception saved');

  $form_state['redirect'] = 'route/' . $form_state['rid'] . '/stops/view';

}

function gtfs_manager_stop_calendar_date_form_delete_submit($form, &$form_state) {

}

/**
 * Form builder; create and display the form for a new stop service feature
 */
function gtfs_manager_stop_service_feature_form($form, &$form_state, $stop_service_feature, $route, $stop) {

  //Set the id to identify this as the stop edit form
  $form['#id'] = 'stop-service-feature-form';

  //fetch active service features
  $stop->service_feature_ids = fetch_features('stop', $stop->sid, TRUE);

  // save the stop and service feature objects for later
  $form['#stop'] = $stop;
  $form_state['stop'] = $stop;
  $form_state['stop_service_feature'] = $stop_service_feature;
  $form_state['route'] = $route;

  $form['create_link'] = array(
    '#markup' => l('Create a new feature', 'service_feature/new')
  );

  //checkboxes for all features
  $form['gtfs_manager_stop_service_feature_ids'] = array(
    '#type' => 'checkboxes',
    '#options' => fetch_service_features(),
    '#title' => t('Select features provided by this stop'),
    '#default_value' => $stop->service_feature_ids,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save Stop'),
    '#weight' => 5,
    '#submit' => array('gtfs_manager_stop_service_features_form_submit'),
  );

  $form['#validate'][] = 'gtfs_manager_stop_service_features_form_validate';

  return $form;
}

function gtfs_manager_stop_service_features_form_validate($form, &$form_state) {

}

function gtfs_manager_stop_service_features_form_submit($form, &$form_state) {
  global $user;

  $service_feature_ids = $form_state['stop']->service_feature_ids;
  $stop_service_feature = $form_state['stop_service_feature'];
  $route = $form_state['route'];

  // if this is a new record
  if (empty($stop_service_feature->created_by)) {
    $stop_service_feature->created_by = $user->name;
  }

  $stop_service_feature->updated_by = $user->name;
  $stop_service_feature->sid = $form_state['stop']->sid;


  foreach ($form_state['values']['gtfs_manager_stop_service_feature_ids'] as $key => $value) {
    //make a copy so we can loop through and save the different features
    $copy = $stop_service_feature;

    if ($value != 0) {
      $copy->service_feature_id = $key;
      //Save
      stop_service_feature_save($copy);
      unset($service_feature_ids[$key]);
    }
  }

  //delete unchecked features
  if (sizeof($service_feature_ids) != 0) {
      $query = 'select stop_service_feature_id from stop_service_features where sid = :sid and service_feature_id in (:sfids)';
      $result = db_query($query, array(':sid' => $stop_service_feature->sid, ':sfids' => implode(',', $service_feature_ids)));
      while ($id = $result->fetchField()) {
        stop_service_feature_delete(array($id));
      }
  }

  drupal_set_message(t('Stop Service Feature(s) saved'));

  $form_state['redirect'] = 'route/' . $route->rid  .'/stops/view';

}

/**
 * Save a new or updated stop service feature
 *
 * @param $stop_service_feature a populated stop object
 */
function stop_service_feature_save($stop_service_feature) {
  return entity_get_controller('stop_service_feature')->save($stop_service_feature);
}

/**
 * deletes a stop service feature via controller
 * @param  $route_service_feature the service feature being deleted
 *
 */
function stop_service_feature_delete($stop_service_feature_id) {
  return entity_get_controller('stop_service_feature')->delete($stop_service_feature_id);
}
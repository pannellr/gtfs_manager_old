<?php

class StopController extends DrupalDefaultEntityController {

  /**
   * create empty stop object
   *
   * @param $sid the stop id, may be used if on update
   *
   * @return the empty Stop object
   */
  public function create($sid = '') {
    return (object) array(
      'sid' => $sid,
      'stop_name' => '',
      'stop_desc' => '',
      'stop_northing' => '',
      'stop_easting' => '',
      'utm_zone' => '',
      'wheelchair_boarding' => '',
      'created_at' => '',
      'updated_at' => '',
      'created_by' => '',
      'updated_by' => '',
      //other table properties
      'service_id' => '',
      'arrival_time' => '',
      'departure_time' => '',
      'stop_sequence' => '',
      'start_date' => '',
      'end_date' => '',
      'days' => array(),
      'monday' => '',
      'teusday' => '',
      'wednesday' => '',
      'thursday' => '',
      'friday' => '',
      'saturday' => '',
      'sunday' => '',
    );
  }

  public function save($stop) {
    $transaction = db_transaction();

    try {
      global $user;
      
      //determine if stop is new
      $stop->is_new = empty($stop->sid);

      //set timestamps
      if (empty($stop->created_at)) {
	$stop->created_at = REQUEST_TIME;
      }

      $stop->updated_at = REQUEST_TIME;

      // give other modules a chance to prepare field data
      field_attach_presave('stop', $stop);

      // if this is a new stop
      if ($stop->is_new) {
	// Save the stop
	drupal_write_record('stops', $stop);
	$op = 'insert';
      } else {
	// update the stop
	drupal_write_record('stops', $stop, 'sid');
	$op = 'update';
      }

      // Save fields
      $function = 'field_attach_' . $op;
      $function('stop', $stop);

      module_invoke_all('entity_' . $op, $stop, 'stop');

      // Clear internal properties
      unset($stop->is_new);

      // Ignore slave server temporarily to give time for insert/update
      db_ignore_slave();

      return $stop;

      //catch exceptions and rollback database changes
    } catch (Exception $e) {
      $transaction->rollback();
      watchdog_exception('stop', $e, NULL, WATCHDOG_ERROR);
      return FALSE;
    }
  }

  public function delete($ids) {
    if (!empty($sids)) {
      $stops = $this->load($sids, array());
      $transaction = db_transaction();

      try {
	db_delete('stops')
	  ->condition('tid', $ids[0], '=')
	  ->condition('sid', $ids[1], '=')
	  ->execute();

	foreach ($stops as $stop_id => $stop) {
	  field_attach_delete('stop', $stop);
	}

	db_ignore_slave();

      } catch (Exception $e) {
	$transaction->rollback();
	watchdog_exception('stop', $e, NULL, WATCHDOG_ERROR);
	return FALSE;
      }

      module_invoke_all('entity_delete', $stop, 'stop');

      //Clear the page, block, and stop caches
      cache_clear_all();
      $this->resetCache();
    }
    return TRUE;
  }

  public function getStops($sids, $conditions) {
    $stops = array();
    $where = '';
    $where_args = array();
    
    if ($sids) {
      if (sizeof($sids) > 1) {
	$where = 's.sid in :sids';
	$where_args = array(':sids' => '(' . join(',', $sids) . ')');
      } else {
	$where = 's.sid = :sid';
	$where_args = array(':sid' => $sids[0]);
      }
    } else {
      
      $where = 't.rid = :rid';
      $where_args = array(':rid' => $conditions[1]);
    }

    $result =
      db_query('select * from {trips} t join {stop_times} st on st.tid = t.tid join {stops} s on s.sid = st.sid join {calendar} c on c.service_id = t.service_id where ' . $where, $where_args);

    if ($result) {
      while ($stop = $result->fetchObject()) {
	$days = array();
	$days['monday'] = ($stop->monday == 1) ? 'monday' : '';
	$days['tuesday'] = ($stop->tuesday == 1) ? 'tuesday' : '';
	$days['wednesday'] = ($stop->monday == 1) ? 'wednesday' : '';
	$days['thursday'] = ($stop->thursday == 1) ? 'thursday' : '';
	$days['friday'] = ($stop->friday == 1) ? 'friday' : '';
	$days['saturday'] = ($stop->saturday == 1) ? 'saturday' : '';
	$days['sunday'] = ($stop->sunday == 1) ? 'sunday' : '';
	$stop->days = $days;
	array_push($stops, $stop);
      }
    }
    return $stops;
  }

}
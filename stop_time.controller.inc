<?php

class StopTimeController extends DrupalDefaultEntityController {

  /**
   * create empty StopTime object
   *
   * @param $sid the stop id
   * @param $tid the trip id
   *
   * @return the empty Route object
   */
  public function create($sid = '', $tid = '') {
    return (object) array(
      'sid' => $sid,
      'tid' => $tid,
      'arrival_time' => '',
      'departure_time' => '',
      'stop_sequence' => '',
      'shape_dist_traveled' => '',
      'created_at' => '',
      'updated_at' => '',
      'created_by' => '',
      'updated_by' => '',
    );
  }

  public function save($stop_time) {
    $transaction = db_transaction();

    try {
      global $user;
      //determine if stop time is new
      $stop_time->is_new = !db_query('select 1 from {stop_times} where sid = :sid and tid = :tid', array(':sid' => $stop_time->sid, ':tid' => $stop_time->tid))
	    ->fetchField();

      //set timestamps
      if (empty($stop_time->created_at)) {
	    $stop_time->created_at = REQUEST_TIME;
      }

      $stop_time->updated_at = REQUEST_TIME;


      // if this is a new stop time
      if ($stop_time->is_new) {
	// Save the route

	//not sure why drupal_write_record isn't working here
	$ids = db_insert('stop_times')
	  ->fields(array(
	    'tid' => $stop_time->tid,
	    'sid' => $stop_time->sid,
	    'arrival_time' => $stop_time->arrival_time,
	    'departure_time' => $stop_time->departure_time,
	    'stop_sequence' => $stop_time->stop_sequence,
	    'shape_dist_traveled' => $stop_time->shape_dist_traveled,
	    'created_at' => $stop_time->created_at,
	    'updated_at' => $stop_time->updated_at,
	    'created_by' => $stop_time->created_by,
	    'updated_by' => $stop_time->updated_by
	  ))
	  ->execute();

	$op = 'insert';
      } else {
	// update the stop time
	$ids = db_update('stop_times')
	  ->fields(array(
	    'arrival_time' => $stop_time->arrival_time,
	    'departure_time' => $stop_time->departure_time,
	    'stop_sequence' => $stop_time->stop_sequence,
	    'shape_dist_traveled' => $stop_time->shape_dist_traveled,
	    'created_at' => $stop_time->created_at,
	    'updated_at' => $stop_time->updated_at,
	    'created_by' => $stop_time->created_by,
	    'updated_by' => $stop_time->updated_by
	  ))
	  ->condition('tid', $stop_time->tid, '=')
	  ->condition('sid', $stop_time->sid, '=')
	  ->execute();

	$op = 'update';
      }

      // Save fields
      $function = 'field_attach_' . $op;
      $function('stop_time', $stop_time);

      module_invoke_all('entity_' . $op, $stop_time, 'stop_time');

      // Clear internal properties
      unset($stop_time->is_new);

      // Ignore slave server temporarily to give time for insert/update
      db_ignore_slave();

      return $stop_time;

    } catch (Exception $e) {
      $transaction->rollback();
      watchdog_exception('stop_time', $e, NULL, WATCHDOG_ERROR);
      return FALSE;
    }
  }

  public function delete($ids) {

    $transaction = db_transaction();

    if (!empty($ids)) {
      try {
	foreach ($ids as $sid_tid) {
	  db_delete('stop_times')
	    ->condition('sid', $sid_tid['sid'], '=')
	    ->condition('tid', $sid_tid['tid'], '=')
	    ->execute();
	}

	db_ignore_slave();
      } catch (Exception $e) {
	$transaction->rollback();
	watchdog_exception('stop_time', $e, NULL, WATCHDOG_ERROR);
	return FALSE;
      }

      //Clear the page, block, and route caches
      cache_clear_all();
      $this->resetCache();
    }
    return TRUE;
  }


}
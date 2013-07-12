<?php

class TripController extends DrupalDefaultEntityController {

  /**
   * create empty route object
   *
   * @param $rid the route id, may be used if on update
   *
   * @return the empty Route object
   */
  public function create($tid = '') {
    return (object) array(
      'tid' => $tid,
      'rid' => '',
      'service_id' => '',
      'created_at' => '',
      'updated_at' => '',
      'created_by' => '',
      'updated_by' => '',
    );
  }

  public function save($trip) {
    $transaction = db_transaction();

    try {
      global $user;
      
      //determine if trip is new
      $trip->is_new = empty($trip->tid);

      //set timestamps
      if (empty($trip->created_at)) {
	$trip->created_at = REQUEST_TIME;
      }

      $trip->updated_at = REQUEST_TIME;

      // give other modules a chance to prepare field data
      field_attach_presave('trip', $trip);

      // if this is a new trip
      if ($trip->is_new) {
	// Save the trip
	drupal_write_record('trips', $trip);
	$op = 'insert';
      } else {
	// update the trip
	drupal_write_record('trips', $trip, 'tid');
	$op = 'update';
      }

      // Save fields
      $function = 'field_attach_' . $op;
      $function('trip', $trip);

      module_invoke_all('entity_' . $op, $trip, 'trip');

      // Clear internal properties
      unset($trip->is_new);

      // Ignore slave server temporarily to give time for insert/update
      db_ignore_slave();

      return $trip;

    } catch (Exception $e) {
      $transaction->rollback();
      watchdog_exception('trip', $e, NULL, WATCHDOG_ERROR);
      return FALSE;
    }
  }

  public function delete($tids) {
    if (!empty($tids)) {
      
      $transaction = db_transaction();

      try {
	db_delete('trips')
	  ->condition('tid', $tids, 'IN')
	  ->execute();

	db_ignore_slave();

      } catch (Exception $e) {
	$transaction->rollback();
	watchdog_exception('trip', $e, NULL, WATCHDOG_ERROR);
	return FALSE;
      }

      //Clear the page, block, and route caches
      cache_clear_all();
      $this->resetCache();
    }
    return TRUE;
  }


}
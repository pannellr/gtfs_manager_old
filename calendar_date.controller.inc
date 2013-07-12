<?php
/**
 * Controller Class for an calendar_date
 */
class CalendarDateController extends DrupalDefaultEntityController {

  public function create($service_id = '') {
    return (object) array(
      'service_id' => $service_id,
      'exception_date' => '',
      'exception_type' => '',
      'created_at' => '',
      'updated_at' => '',
      'created_by' => '',
      'updated_by' => '',
    );
  }

  public function save($calendar_date) {

    $transaction = db_transaction();

    try {
      global $user;

      $is_new = TRUE; 

      //set timestamps
      if (empty($calendar_date->created_at)) {
	$calendar_date->created_at = REQUEST_TIME;
      } else {
	$is_new = FALSE;
      }

      $calendar_date->updated_at = REQUEST_TIME;

      if ($is_new) {
	drupal_write_record('calendar_dates', $calendar_date);
      } else {
	drupal_write_record('calendar_dates', $calendar_date, 'service_id');
      }
      // Ignore slave server temporarily to give time for insert/update
      db_ignore_slave();

      return $calendar_date;
    } catch (Exception $e) {
      $transaction->rollback();
      watchdog_exception('calendar_date', $e, NULL, WATCHDOG_ERROR);
      return FALSE;
    }
  }

  public function delete($service_ids) {
    if (!empty($service_ids)) {

      $transaction = db_transaction();

      try {
	db_delete('calendar_date')
	  ->condition('service_id', $service_ids, 'IN')
	  ->execute();

	db_ignore_slave();

      } catch (Exception $e) {
	$transaction->rollback();
	watchdog_exception('calendar_date', $e, NULL, WATCHDOG_ERROR);
	return FALSE;
      }

      //Clear the page, block, and calendar_date caches
      cache_clear_all();
      $this->resetCache();
    }
    return TRUE;
  }


}
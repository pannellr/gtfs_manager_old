<?php
/**
 * Controller Class for an calendar
 */
class CalendarController extends DrupalDefaultEntityController {

  public function create($service_id = '') {
    return (object) array(
      'service_id' => $service_id,
      'monday' => '0',
      'teusday' => '0',
      'wednesday' => '0',
      'thursday' => '0',
      'friday' => '0',
      'saturday' => '0',
      'sunday' => '0',
      'start_date' => '',
      'end_date' => '',
      'created_at' => '',
      'updated_at' => '',
      'created_by' => '',
      'updated_by' => '',
    );
  }

  public function save($calendar) {
    $transaction = db_transaction();

    try {
      global $user;

      //determine if calendar is new
      $calendar->is_new = empty($calendar->service_id);

      //set timestamps
      if (empty($calendar->created_at)) {
	$calendar->created_at = REQUEST_TIME;
      }

      $calendar->updated_at = REQUEST_TIME;

      // give other modules a chance to prepare field data
      field_attach_presave('calendar', $calendar);

      // if this is a new calendar
      if ($calendar->is_new) {
	// Save the calendar
	drupal_write_record('calendar', $calendar);
	$op = 'insert';
      } else {
	// update the calendar
	drupal_write_record('calendar', $calendar, 'service_id');
	$op = 'update';
      }

      // Save fields
      $function = 'field_attach_' . $op;
      $function('calendar', $calendar);

      module_invoke_all('entity_' . $op, $calendar, 'calendar');

      // Clear internal properties
      unset($calendar->is_new);

      // Ignore slave server temporarily to give time for insert/update
      db_ignore_slave();

      return $calendar;
     

    } catch (Exception $e) {
      $transaction->rollback();
      watchdog_exception('calendar', $e, NULL, WATCHDOG_ERROR);
      return FALSE;
    }
  }

  public function delete($service_ids) {
    if (!empty($service_ids)) {

      $transaction = db_transaction();

      try {
	db_delete('calendar')
	  ->condition('service_id', $service_ids, 'IN')
	  ->execute();

	db_ignore_slave();

      } catch (Exception $e) {
	$transaction->rollback();
	watchdog_exception('calendar', $e, NULL, WATCHDOG_ERROR);
	return FALSE;
      }

      //Clear the page, block, and calendar caches
      cache_clear_all();
      $this->resetCache();
    }
    return TRUE;
  }


}
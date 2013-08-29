<?php

class FareController extends DrupalDefaultEntityController {

  /**
   * create empty fare object
   *
   * @param $fare_id the fare id, may be used if on update
   *
   * @return the empty Fare object
   */

  public function create($fare_id = '') {
    return (object) array(
      'fare_id' => $fare_id,
      'price' => '',
      'currency_type_id' => '',
      'payment_method' => '',
      'transfers' => '',
      'created_at' => '',
      'updated_at' => '',
      'created_by' => '',
      'updated_by' => '',
    );
  }

  public function save($fare) {
    $transaction = db_transaction();

    try {
      global $user;

      //determine if fare is new
      $fare->is_new = empty($fare->fare_id);

      //set timestamps
      if (empty($fare->created_at)) {
	    $fare->created_at = REQUEST_TIME;
      }

      $fare->updated_at = REQUEST_TIME;

      // give other modules a chance to prepare field data
      field_attach_presave('fare', $fare);

      // if this is a new fare
      if ($fare->is_new) {
	    // Save the fare
	    drupal_write_record('fare_attributes', $fare);
	    $op = 'insert';
      } else {
	    // update the fare
	    drupal_write_record('fare_attributes', $fare, 'fare_id');
	    $op = 'update';
      }

      // Clear internal properties
      unset($fare->is_new);

      // Ignore slave server temporarily to give time for insert/update
      db_ignore_slave();

      return $fare;

    } catch (Exception $e) {
      $transaction->rollback();
      watchdog_exception('fare', $e, NULL, WATCHDOG_ERROR);
      return FALSE;
    }
  }

  public function delete($fare_ids) {
    if (!empty($fare_ids)) {
      $fares = $this->load($fare_ids, array());
      $transaction = db_transaction();

      try {
	db_delete('fare_attributes')
	  ->condition('fare_id', $fare_ids, 'IN')
	  ->execute();

	foreach ($fares as $fare_id => $fare) {
	  field_attach_delete('fare', $fare);
	}

	db_ignore_slave();

      } catch (Exception $e) {
	$transaction->rollback();
	watchdog_exception('fare', $e, NULL, WATCHDOG_ERROR);
	return FALSE;
      }

      module_invoke_all('entity_delete', $fare, 'fare');

      //Clear the page, block, and fare caches
      cache_clear_all();
      $this->resetCache();
    }
    return TRUE;
  }

}
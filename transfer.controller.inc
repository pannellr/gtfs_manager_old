<?php
/**
 * Controller Class for aa transfer
 */
class TransferController extends DrupalDefaultEntityController {


  public function create() {
    return (object) array(
      'from_stop_id' => '',
      'to_stop_id' => '',
      'transfer_type' => '',
      'min_transfer_time' => '',
      'is_active' => 0,
      'created_at' => '',
      'updated_at' => '',
      'created_by' => '',
      'updated_by' => '',
    );
  }

  public function save($transfer) {
    $transaction = db_transaction();

    try {
      global $user;

      $transfer->created_by = $user->name;
      $transfer->updated_by = $user->name;

      // Save the transfer
      drupal_write_record('transfers', $transfer);

      // Ignore slave server temporarily to give time for insert/update
      db_ignore_slave();

      return $transfer;

    } catch (Exception $e) {
      $transaction->rollback();
      watchdog_exception('transfer', $e, NULL, WATCHDOG_ERROR);
      return FALSE;
    }
  }

  public function load($ids = array(), $conditions = array()) {
    return db_select('transfers', 't')
      ->fields('t')
      ->condition('from_stop_id', $ids['from_stop_id'], '=')
      ->condition('to_stop_id', $ids['to_stop_id'], '=')
      ->execute()
      ->fetchObject();
  }

  public function delete($transfer) {
    if (!empty($transfer)) {
      try {
	    $result = db_delete('transfers')
	      ->condition('from_stop_id', $transfer->from_stop_id, '=')
	      ->condition('to_stop_id', $transfer->to_stop_id, '=')
	      ->execute();

	    db_ignore_slave();

      } catch (Exception $e) {
	    $transaction->rollback();
	    watchdog_exception('transfer', $e, NULL, WATCHDOG_ERROR);
	    return FALSE;
      }

      //Clear the page, block, and agency caches
      cache_clear_all();
      $this->resetCache();
    }
    return TRUE;
  }

}
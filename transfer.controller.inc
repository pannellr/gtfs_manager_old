<?php
/**
 * Controller Class for aa transfer
 */
class TransferController extends DrupalDefaultEntityController {


  public function create() {
    return (object) array(
      'fromt_stop_id' => '',
      'to_stop_id' => '',
      'transfer_type' => '',
      'min_transfer_time' => '',
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

      // Save the agency
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


}
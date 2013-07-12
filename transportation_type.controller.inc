<?php

class TransportationTypeController extends DrupalDefaultEntityController {
  
  /**
   * create empty transportation_type object
   *
   * @param $transportation_type_id the transportation_type id, may be used if on update
   *
   * @return the empty Transportation_Type object
   */

  public function create($transportation_type_id = '') {
    return (object) array(
      'transportation_type_id' => $transportation_type_id,
      'transportation_type_name' => '',
      'created_at' => '',
      'updated_at' => '',
      'created_by' => '',
      'updated_by' => '',
    );
  }

  public function save($transportation_type) {
    $transaction = db_transaction();

    try {
      global $user;
      
      //determine if transportation_type is new
      $transportation_type->is_new 
	= empty($transportation_type->transportation_type_id);

      //set timestamps
      if (empty($transportation_type->created_at)) {
	$transportation_type->created_at = REQUEST_TIME;
      }

      $transportation_type->updated_at = REQUEST_TIME;

      // give other modules a chance to prepare field data
      field_attach_presave('transportation_type', $transportation_type);

      // if this is a new transportation_type
      if ($transportation_type->is_new) {
	// Save the transportation_type
	drupal_write_record('transportation_types', $transportation_type);
	$op = 'insert';
      } else {
	// update the transportation_type
	drupal_write_record('transportation_types', $transportation_type, 
			    'transportation_type_id');
	$op = 'update';
      }

      // Save fields
      $function = 'field_attach_' . $op;
      $function('transportation_type', $transportation_type);

      module_invoke_all('entity_' . $op, $transportation_type, 'transportation_type');

      // Clear internal properties
      unset($transportation_type->is_new);

      // Ignore slave server temporarily to give time for insert/update
      db_ignore_slave();

      return $transportation_type;

    } catch (Exception $e) {
      $transaction->rollback();
      watchdog_exception('transportation_type', $e, NULL, WATCHDOG_ERROR);
      return FALSE;
    }
  }

  public function delete($transportation_type_ids) {
    if (!empty($transportation_type_ids)) {
      $transportation_types = $this->load($transportation_type_ids, array());
      $transaction = db_transaction();

      try {
	db_delete('transportation_types')
	  ->condition('transportation_type_id', $transportation_type_ids, 'IN')
	  ->execute();

	foreach ($transportation_types as $transportation_type_id => $transportation_type) {
	  field_attach_delete('transportation_type', $transportation_type);
	}

	db_ignore_slave();

      } catch (Exception $e) {
	$transaction->rollback();
	watchdog_exception('transportation_type', $e, NULL, WATCHDOG_ERROR);
	return FALSE;
      }

      module_invoke_all('entity_delete', $transportation_type, 'transportation_type');

      //Clear the page, block, and transportation_type caches
      cache_clear_all();
      $this->resetCache();
    }
    return TRUE;
  }

}
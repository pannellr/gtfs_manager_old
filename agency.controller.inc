<?php
/**
 * Controller Class for an agency
 */
class AgencyController extends DrupalDefaultEntityController {

  public function create($aid = '') {
    return (object) array(
      'aid' => $aid,
      'agency_name' => '',
      'agency_url' => '',
      'agency_phone' => '',
      'agency_email' => '',
      'agency_info' => '',
      'agency_fare_url' => '',
      'time_zone_id' => '',
      'agency_fare_type' => '',
      'created_at' => '',
      'updated_at' => '',
      'created_by' => '',
      'updated_by' => '',
    );
  }

  public function save($agency) {
    $transaction = db_transaction();

    try {
      global $user;

      //determine if agency is new
      $agency->is_new = empty($agency->aid);

      //set timestamps
      if (empty($agency->created_at)) {
	    $agency->created_at = REQUEST_TIME;
      }

      $agency->updated_at = REQUEST_TIME;

      // give other modules a chance to prepare field data
      field_attach_presave('agency', $agency);

      // if this is a new agency
      if ($agency->is_new) {
	    // Save the agency
	    drupal_write_record('agencies', $agency);
	    $op = 'insert';
      } else {
	    // update the agency
	    drupal_write_record('agencies', $agency, 'aid');
	    $op = 'update';
      }

      // Save fields
      $function = 'field_attach_' . $op;
      $function('agency', $agency);

      module_invoke_all('entity_' . $op, $agency, 'agency');

      // Clear internal properties
      unset($agency->is_new);

      // Ignore slave server temporarily to give time for insert/update
      db_ignore_slave();

      return $agency;

    } catch (Exception $e) {
      $transaction->rollback();
      watchdog_exception('agency', $e, NULL, WATCHDOG_ERROR);
      return FALSE;
    }
  }

  public function delete($aids) {
    if (!empty($aids)) {
      $agencies = $this->load($aids, array());
      $transaction = db_transaction();

      try {
	    db_delete('agencies')
	    ->condition('aid', $aids, 'IN')
	    ->execute();

	    foreach ($agencies as $agency_id => $agency) {
	      field_attach_delete('agency', $agency);
	    }

	    db_ignore_slave();

        } catch (Exception $e) {
      	  $transaction->rollback();
    	  watchdog_exception('agency', $e, NULL, WATCHDOG_ERROR);
    	  return FALSE;
        }

      module_invoke_all('entity_delete', $agency, 'agency');

      //Clear the page, block, and agency caches
      cache_clear_all();
      $this->resetCache();
    }
    return TRUE;
  }


}
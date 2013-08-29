<?php

class StopServiceFeatureController extends DrupalDefaultEntityController {

  /**
   * create empty stop_service_feature object
   *
   * @param $stop_service_feature_id the stop_service_feature id,
   * may be used if on update
   *
   * @return the empty Stop_Service_Feature object
   */
  public function create($stop_service_feature_id = '') {
    return (object) array(
      'stop_service_feature_id' => $stop_service_feature_id,
      'service_feature_id' => '',
      'created_at' => '',
      'updated_at' => '',
      'created_by' => '',
      'updated_by' => '',
    );
  }

  public function save($stop_service_feature) {
    $transaction = db_transaction();

    try {
      global $user;

      //determine if stop_service_feature is new
      $stop_service_feature->is_new = empty($stop_service_feature->stop_service_feature_id);

      //set timestamps
      if (empty($stop_service_feature->created_at)) {
	    $stop_service_feature->created_at = REQUEST_TIME;
      }

      $stop_service_feature->updated_at = REQUEST_TIME;

      // if this is a new stop_service_feature
      if ($stop_service_feature->is_new) {
	// Save the stop_service_feature
	  drupal_write_record('stop_service_features', $stop_service_feature);
      } else {
	// update the stop_service_feature
	  drupal_write_record('stop_service_features', $stop_service_feature, 'stop_service_feature_id');
      }

      // Clear internal properties
      unset($stop_service_feature->is_new);

      // Ignore slave server temporarily to give time for insert/update
      db_ignore_slave();

      return $stop_service_feature;

    } catch (Exception $e) {
      $transaction->rollback();
      watchdog_exception('stop_service_feature', $e, NULL, WATCHDOG_ERROR);
      return FALSE;
    }
  }

  public function delete($stop_service_feature_ids) {
    if (!empty($stop_service_feature_ids)) {
      $transaction = db_transaction();

      try {
        	db_delete('stop_service_features')
	          ->condition('stop_service_feature_id', $stop_service_feature_ids, 'IN')
	          ->execute();

	        db_ignore_slave();

      } catch (Exception $e) {
	    $transaction->rollback();
	    watchdog_exception('stop_service_feature', $e, NULL, WATCHDOG_ERROR);
	    return FALSE;
      }

      //Clear the page, block, and stop_service_feature caches
      cache_clear_all();
      $this->resetCache();
    }
    return TRUE;
  }

}
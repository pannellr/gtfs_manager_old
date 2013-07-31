<?php

class AgencyServiceFeatureController extends DrupalDefaultEntityController {

  /**
   * create empty agency_service_feature object
   *
   * @param $agency_service_feature_id the agency_service_feature id, may be used if on update
   *
   * @return the empty Agency_Service_Feature object
   */
  public function create($agency_service_feature_id = '') {
    return (object) array(
      'agency_service_feature_id' => $agency_service_feature_id,
      'service_feature_id' => '',
      'created_at' => '',
      'updated_at' => '',
      'created_by' => '',
      'updated_by' => '',
    );
  }

  public function save($agency_service_feature) {
    $transaction = db_transaction();

    try {
      global $user;

      //determine if agency_service_feature is new
      $agency_service_feature->is_new = empty($agency_service_feature->agency_service_feature_id);

      //set timestamps
      if (empty($agency_service_feature->created_at)) {
	    $agency_service_feature->created_at = REQUEST_TIME;
      }

      $agency_service_feature->updated_at = REQUEST_TIME;

      // if this is a new agency_service_feature
      if ($agency_service_feature->is_new) {
	    // Save the agency_service_feature
	    drupal_write_record('agency_service_features', $agency_service_feature);
      } else {
	    // update the agency_service_feature
	   drupal_write_record('agency_service_features', $agency_service_feature, 'agency_service_feature_id');
      }

      // Clear internal properties
      unset($agency_service_feature->is_new);

      // Ignore slave server temporarily to give time for insert/update
      db_ignore_slave();

      return $agency_service_feature;

    } catch (Exception $e) {
      $transaction->rollback();
      watchdog_exception('agency_service_feature', $e, NULL, WATCHDOG_ERROR);
      return FALSE;
    }
  }

  public function delete($agency_service_feature_ids) {
    if (!empty($agency_service_feature_ids)) {

      $transaction = db_transaction();

      watchdog('id', serialize($agency_service_feature_ids));

      try {
	    db_delete('agency_service_features')
	      ->condition('agency_service_feature_id', $agency_service_feature_ids, 'IN')
	      ->execute();

	    db_ignore_slave();

      } catch (Exception $e) {
    	$transaction->rollback();
    	watchdog_exception('agency_service_feature', $e, NULL, WATCHDOG_ERROR);
    	return FALSE;
      }

      //Clear the page, block, and agency_service_feature caches
      cache_clear_all();
      $this->resetCache();
    }
    return TRUE;
  }

}
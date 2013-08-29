<?php

class RouteServiceFeatureController extends DrupalDefaultEntityController {

  /**
   * create empty route_service_feature object
   *
   * @param $route_service_feature_id the route_service_feature id, may be used if on update
   *
   * @return the empty Route_Service_Feature object
   */
  public function create($route_service_feature_id = '') {
    return (object) array(
      'route_service_feature_id' => $route_service_feature_id,
      'service_feature_id' => '',
      'created_at' => '',
      'updated_at' => '',
      'created_by' => '',
      'updated_by' => '',
    );
  }

  public function save($route_service_feature) {
    $transaction = db_transaction();

    try {
      global $user;

      //determine if route_service_feature is new
      $route_service_feature->is_new = empty($route_service_feature->route_service_feature_id);

      //set timestamps
      if (empty($route_service_feature->created_at)) {
	$route_service_feature->created_at = REQUEST_TIME;
      }

      $route_service_feature->updated_at = REQUEST_TIME;

      // give other modules a chance to prepare field data
      field_attach_presave('route_service_feature', $route_service_feature);

      // if this is a new route_service_feature
      if ($route_service_feature->is_new) {
	// Save the route_service_feature
	drupal_write_record('route_service_features', $route_service_feature);
      } else {
	// update the route_service_feature
	drupal_write_record('route_service_features', $route_service_feature, 'route_service_feature_id');
      }

      // Clear internal properties
      unset($route_service_feature->is_new);

      // Ignore slave server temporarily to give time for insert/update
      db_ignore_slave();

      return $route_service_feature;

    } catch (Exception $e) {
      $transaction->rollback();
      watchdog_exception('route_service_feature', $e, NULL, WATCHDOG_ERROR);
      return FALSE;
    }
  }

  public function delete($route_service_feature_ids) {
    if (!empty($route_service_feature_ids)) {
      $transaction = db_transaction();

      try {
	    db_delete('route_service_features')
	      ->condition('route_service_feature_id', $route_service_feature_ids, 'IN')
	      ->execute();

	    db_ignore_slave();

      } catch (Exception $e) {
    	$transaction->rollback();
    	watchdog_exception('route_service_feature', $e, NULL, WATCHDOG_ERROR);
    	return FALSE;
      }

      //Clear the page, block, and agency_service_feature caches
      cache_clear_all();
      $this->resetCache();
    }
    return TRUE;
  }

}
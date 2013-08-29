<?php

class ServiceFeatureController extends DrupalDefaultEntityController {

  /**
   * create empty service_feature object
   *
   * @param $service_feature_id the service_feature id, may be used if on update
   *
   * @return the empty service_seature object
   */
  public function create($service_feature_id = '') {
    return (object) array(
      'service_feature_id' => $service_feature_id,
      'service_feature_type_id' => '',
      'service_feature_name' => '',
      'service_feature_icon_url' => '',
      'service_feature_info' => '',
      'created_at' => '',
      'updated_at' => '',
      'created_by' => '',
      'updated_by' => '',
    );
  }

  public function save($service_feature) {
    $transaction = db_transaction();

    try {
      global $user;
      
      //determine if service_feature is new
      $service_feature->is_new = empty($service_feature->service_feature_id);

      //set timestamps
      if (empty($service_feature->created_at)) {
	$service_feature->created_at = REQUEST_TIME;
      }

      $service_feature->updated_at = REQUEST_TIME;

      // give other modules a chance to prepare field data
      field_attach_presave('service_feature', $service_feature);

      // if this is a new service_feature
      if ($service_feature->is_new) {
	// Save the service_feature
	drupal_write_record('service_features', $service_feature);
	$op = 'insert';
      } else {
	// update the service_feature
	drupal_write_record('service_features', $service_feature, 'service_feature_id');
	$op = 'update';
      }

      // Save fields
      $function = 'field_attach_' . $op;
      $function('service_feature', $service_feature);

      module_invoke_all('entity_' . $op, $service_feature, 'service_feature');

      // Clear internal properties
      unset($service_feature->is_new);

      // Ignore slave server temporarily to give time for insert/update
      db_ignore_slave();

      return $service_feature;

    } catch (Exception $e) {
      $transaction->rollback();
      watchdog_exception('service_feature', $e, NULL, WATCHDOG_ERROR);
      return FALSE;
    }
  }

  public function delete($service_feature_ids) {
    if (!empty($service_feature_ids)) {
      $service_features = $this->load($service_feature_ids, array());
      $transaction = db_transaction();

      try {
	db_delete('service_features')
	  ->condition('service_feature_id', $service_feature_ids, 'IN')
	  ->execute();

	foreach ($service_features as $service_feature_id => $service_feature) {
	  field_attach_delete('service_feature', $service_feature);
	}

	db_ignore_slave();

      } catch (Exception $e) {
	$transaction->rollback();
	watchdog_exception('service_feature', $e, NULL, WATCHDOG_ERROR);
	return FALSE;
      }

      module_invoke_all('entity_delete', $service_feature, 'service_feature');

      //Clear the page, block, and service_feature caches
      cache_clear_all();
      $this->resetCache();
    }
    return TRUE;
  }

}
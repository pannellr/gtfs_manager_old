<?php
/**
 * Controller Class for an service area point
 */
class ServiceAreaPointController extends DrupalDefaultEntityController {

  public function create($service_area_point_id = '') {
    return (object) array(
      'service_area_point_id' => $service_area_point_id,
      'service_area_id' => '',
      'shape_point_lat' => '',
      'shape_point_long' => '',
      'shape_point_sequence' => '',
      'created_at' => '',
      'updated_at' => '',
      'created_by' => '',
      'updated_by' => '',
    );
  }

  public function save($service_area_point) {
    $transaction = db_transaction();

    try {
      global $user;

      //determine if service_area_point is new
      $service_area_point->is_new = empty($service_area_point->service_area_point_id);

      //set timestamps
      if (empty($service_area_point->created_at)) {
	$service_area_point->created_at = REQUEST_TIME;
      }

      $service_area_point->updated_at = REQUEST_TIME;

      // give other modules a chance to prepare field data
      field_attach_presave('service_area_point', $service_area_point);

      // if this is a new service_area_point
      if ($service_area_point->is_new) {
	// Save the service_area_point
	drupal_write_record('service_area_points', $service_area_point);
	$op = 'insert';
      } else {
	// update the service_area_point
	drupal_write_record('service_area_points', $service_area_point, 'service_area_point_id');
	$op = 'update';
      }

      // Save fields
      $function = 'field_attach_' . $op;
      $function('service_area_point', $service_area_point);

      module_invoke_all('entity_' . $op, $service_area_point, 'service_area_point');

      // Clear internal properties
      unset($service_area_point->is_new);

      // Ignore slave server temporarily to give time for insert/update
      db_ignore_slave();

      return $service_area_point;

    } catch (Exception $e) {
      $transaction->rollback();
      watchdog_exception('service_area_point', $e, NULL, WATCHDOG_ERROR);
      return FALSE;
    }
  }

  public function delete($service_area_point_ids) {
    if (!empty($service_area_point_ids)) {
      $service_area_points = $this->load($service_area_point_ids, array());
      $transaction = db_transaction();

      try {
	db_delete('service_area_points')
	  ->condition('service_area_point_id', $service_area_point_ids, 'IN')
	  ->execute();

	foreach ($service_area_points as $service_area_point_id => $service_area_point) {
	  field_attach_delete('service_area_point', $service_area_point);
	}

	db_ignore_slave();

      } catch (Exception $e) {
	$transaction->rollback();
	watchdog_exception('service_area_point', $e, NULL, WATCHDOG_ERROR);
	return FALSE;
      }

      module_invoke_all('entity_delete', $service_area_point, 'service_area_point');

      //Clear the page, block, and service_area_point caches
      cache_clear_all();
      $this->resetCache();
    }
    return TRUE;
  }

}
<?php
/**
 * Controller Class for a service area
 */
class ServiceAreaController extends DrupalDefaultEntityController {

  public function create($service_area_id = '') {
    return (object) array(
      'service_area_id' => $service_area_id,
      'rid' => '',
      'created_at' => '',
      'updated_at' => '',
      'created_by' => '',
      'updated_by' => '',
    );
  }

  public function save($service_area) {
    $transaction = db_transaction();

    try {
      global $user;

      //determine if service_area is new
      $service_area->is_new = empty($service_area->service_area_id);

      //set timestamps
      if (empty($service_area->created_at)) {
	$service_area->created_at = REQUEST_TIME;
      }

      $service_area->updated_at = REQUEST_TIME;

      // give other modules a chance to prepare field data
      field_attach_presave('service_area', $service_area);

      // if this is a new service_area
      if ($service_area->is_new) {
	// Save the service_area
	drupal_write_record('routes_service_areas', $service_area);
	$op = 'insert';
      } else {
	// update the service_area
	drupal_write_record('routes_service_areas', $service_area, 'service_area_id');
	$op = 'update';
      }

      // Save fields
      $function = 'field_attach_' . $op;
      $function('service_area', $service_area);

      module_invoke_all('entity_' . $op, $service_area, 'service_area');

      // Clear internal properties
      unset($service_area->is_new);

      // Ignore slave server temporarily to give time for insert/update
      db_ignore_slave();

      return $service_area;

    } catch (Exception $e) {
      $transaction->rollback();
      watchdog_exception('service_area', $e, NULL, WATCHDOG_ERROR);
      return FALSE;
    }
  }

  public function delete($service_area_ids) {
    if (!empty($service_area_ids)) {
      $routes_service_areas = $this->load($service_area_ids, array());
      $transaction = db_transaction();

      try {
	db_delete('routes_service_areas')
	  ->condition('service_area_id', $service_area_ids, 'IN')
	  ->execute();

	foreach ($routes_service_areas as $service_area_id => $service_area) {
	  field_attach_delete('service_area', $service_area);
	}

	db_ignore_slave();

      } catch (Exception $e) {
	$transaction->rollback();
	watchdog_exception('service_area', $e, NULL, WATCHDOG_ERROR);
	return FALSE;
      }

      module_invoke_all('entity_delete', $service_area, 'service_area');

      //Clear the page, block, and service_area caches
      cache_clear_all();
      $this->resetCache();
    }
    return TRUE;
  }


}
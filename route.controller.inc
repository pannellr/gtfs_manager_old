<?php

class RouteController extends DrupalDefaultEntityController {

  /**
   * create empty route object
   *
   * @param $rid the route id, may be used if on update
   *
   * @return the empty Route object
   */
  public function create($rid = '') {
    return (object) array(
      'rid' => $rid,
      'aid' => '',
      'transportation_type_id' => '',
      'route_short_name' => '',
      'route_long_name' => '',
      'route_desc' => '',
      'route_type_id' => '',
      'route_url' => '',
      'created_at' => '',
      'updated_at' => '',
      'created_by' => '',
      'updated_by' => '',
    );
  }

  public function save($route) {
    $transaction = db_transaction();

    try {
      global $user;
      
      //determine if route is new
      $route->is_new = empty($route->rid);

      //set timestamps
      if (empty($route->created_at)) {
	$route->created_at = REQUEST_TIME;
      }

      $route->updated_at = REQUEST_TIME;

      // give other modules a chance to prepare field data
      field_attach_presave('route', $route);

      // if this is a new route
      if ($route->is_new) {
	// Save the route
	drupal_write_record('routes', $route);
	$op = 'insert';
      } else {
	// update the route
	drupal_write_record('routes', $route, 'rid');
	$op = 'update';
      }

      // Save fields
      $function = 'field_attach_' . $op;
      $function('route', $route);

      module_invoke_all('entity_' . $op, $route, 'route');

      // Clear internal properties
      unset($route->is_new);

      // Ignore slave server temporarily to give time for insert/update
      db_ignore_slave();

      return $route;

    } catch (Exception $e) {
      $transaction->rollback();
      watchdog_exception('route', $e, NULL, WATCHDOG_ERROR);
      return FALSE;
    }
  }

  public function delete($rids) {
    if (!empty($rids)) {
      $routes = $this->load($rids, array());
      $transaction = db_transaction();

      try {
	db_delete('routes')
	  ->condition('rid', $rids, 'IN')
	  ->execute();

	foreach ($routes as $route_id => $route) {
	  field_attach_delete('route', $route);
	}

	db_ignore_slave();

      } catch (Exception $e) {
	$transaction->rollback();
	watchdog_exception('route', $e, NULL, WATCHDOG_ERROR);
	return FALSE;
      }

      module_invoke_all('entity_delete', $route, 'route');

      //Clear the page, block, and route caches
      cache_clear_all();
      $this->resetCache();
    }
    return TRUE;
  }

}
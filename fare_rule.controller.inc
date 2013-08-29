<?php

class FareRuleController extends DrupalDefaultEntityController {

  /**
   * create empty fare_rule object
   *
   * @param $fare_rule_id the fare_rule id, may be used if on update
   *
   * @return the empty Fare_Rule object
   */

  public function create($fare_id = FALSE) {
    return (object) array(
      'fare_id' => $fare_id,
      'route_id' => '',
      'origin_id' => '',
      'destination_id' => '',
      'contains_id' => '',
      'created_at' => '',
      'updated_at' => '',
      'created_by' => '',
      'updated_by' => '',
    );
  }

  public function save($fare_rule) {
    $transaction = db_transaction();

    try {
      global $user;

      $is_update = FALSE;
      $where = '';
      $args = array();
      $update = db_update('fare_rules')
        ->fields(array('fare_id' => $fare_rule->fare_id));

      $fare_rule->created_at = REQUEST_TIME;
      $fare_rule->updated_at = REQUEST_TIME;
      $fare_rule->updated_by = $user->name;

      if ($fare_rule->origin_id && $fare_rule->destination_id) {
        $where = 'origin_id = :o and destination_id = :d';
        $args = array(':o' => $fare_rule->origin_id, ':d' => $fare_rule->destination_id);
        $update->condition('origin_id', $fare_rule->origin_id, '=')
	      ->condition('destination_id', $fare_rule->destination_id, '=');

      } else if ($fare_rule->contains_id) {
        $where = 'contains_id = :c';
        $args = array(':c' => $fare_rule->contains_id);
        $update->condition('contains_id', $fare_rule->contains_id, '=');

      } else if ($fare_rule->route_id) {
        $where = 'route_id = :r';
        $args = array(':r' => $fare_rule->route_id);
        $update->condition('route_id', $fare_rule->route_id, '=');

      }

        $query = 'select 1 from fare_rules where ' . $where;
        $result = db_query($query, $args);
        $is_update = $result->fetchField();


        if ($is_update) {
	        $update->execute();
        } else {
          $fare_rule->created_by = $user->name;
	      drupal_write_record('fare_rules', $fare_rule);
        }


      // Ignore slave server temporarily to give time for insert/update
      db_ignore_slave();

      return $fare_rule;

    } catch (Exception $e) {
      $transaction->rollback();
      watchdog_exception('fare_rule', $e, NULL, WATCHDOG_ERROR);
      return FALSE;
    }
  }

  public function delete($fare_ids) {
    if (!empty($fare_ids)) {
      $fare_rules = $this->load($fare_ids, array());
      $transaction = db_transaction();

      try {
	db_delete('fare_rules')
	  ->condition('fare_id', $fare_ids, 'IN')
	  ->execute();

	db_ignore_slave();

      } catch (Exception $e) {
	$transaction->rollback();
	watchdog_exception('fare_rule', $e, NULL, WATCHDOG_ERROR);
	return FALSE;
      }

      module_invoke_all('entity_delete', $fare_rule, 'fare_rule');

      //Clear the page, block, and fare_rule caches
      cache_clear_all();
      $this->resetCache();
    }
    return TRUE;
  }

}
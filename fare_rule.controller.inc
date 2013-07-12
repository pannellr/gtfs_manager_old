<?php

class FareRuleController extends DrupalDefaultEntityController {
  
  /**
   * create empty fare_rule object
   *
   * @param $fare_rule_id the fare_rule id, may be used if on update
   *
   * @return the empty Fare_Rule object
   */

  public function create($fare_id = '') {
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

    watchdog('fare_rule', serialize($fare_rule));

    try {
      global $user;
      
      $fare_rule->created_at = REQUEST_TIME;
      $fare_rule->updated_at = REQUEST_TIME;
      
      $query = 
	'select 1 from fare_rules where origin_id = :o and destination_id = :d';
      
      $result = db_query($query, 
			 array(':o' => $fare_rule->origin_id,
			       ':d' => $fare_rule->destination_id
			       )
			 );

      $is_update = $result->fetchField();
	
      watchdog('is update', $is_update);

      if ($is_update) {

	db_update('fare_rules')
	  ->fields(array('fare_id' => $fare_rule->fare_id))
	  ->condition('origin_id', $fare_rule->origin_id, '=')
	  ->condition('destination_id', $fare_rule->destination_id, '=')
	  ->execute();

      } else {
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
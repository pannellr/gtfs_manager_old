<?php
/**
 * Controller Class for collecting and displaying user searches
 */
class UserSearchController extends DrupalDefaultEntityController {

  public function create($user_search_id = '') {
    return (object) array(
      'user_search_id' => $user_search_id,
      'origin_string' => '',
      'origin_string_type_id' => '',
      'destination_string' => '',
      'destination_string_type_id' => '',
      'travel_date' => '',
      'return_date' => '',
      'language' => '',
      'ip' => '',
      'geocode' => '',
      'num_results' => '',
      'return_num_results' => '',
      'created_at' => '',
      'updated_at' => '',
      'created_by' => '',
      'updated_by' => '',
    );
  }

public function save($user_search) {
    $transaction = db_transaction();

    try {
      global $user;      

      $user_search->created_by = isset($user->name) ? $user->name : 'anonymous';
      $user_search->updated_by = isset($user->name) ? $user->name : 'anonymous';

      $user_search->created_at = REQUEST_TIME;
      $user_search->updated_at = REQUEST_TIME;

      drupal_write_record('user_searches', $user_search);

      // Ignore slave server temporarily to give time for insert/update
      db_ignore_slave();

      return $user_search;

    } catch (Exception $e) {
      $transaction->rollback();
      watchdog_exception('user_search', $e, NULL, WATCHDOG_ERROR);
      return FALSE;
    }
  }

}
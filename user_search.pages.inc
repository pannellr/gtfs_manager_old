<?php


function gtfs_manager_user_search_report() {
  $user_searches = entity_load('user_search');

  $header = array();
  $rows = array();
  $i = 0;
  foreach ($user_searches as $user_search) {
    $row = array();
    foreach ($user_search as $col => $value) {
      if ($i == 0) {
	array_push($header, $col);
      }
      array_push($row, array('data' => $value));
    }
    array_push($rows, $row);
    $i++;
  }
  
  return theme('table', array('header' => $header, 'rows' => $rows));

}
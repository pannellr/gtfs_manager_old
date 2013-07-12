<?php

function gtfs_manager_show_time_zones() {

  $result = db_select('time_zones', 'tz')
    ->fields('tz')
    ->execute();

  $header = array();
  $rows = array();
  $i = 0;
  while ($time_zone = $result->fetchObject()) {
    $row = array();
    foreach ($time_zone as $col => $value) {
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


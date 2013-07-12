<?php

function gtfs_manager_show_provinces() {

  $result = db_select('provinces', 'p')
    ->fields('p')
    ->execute();

  $header = array();
  $rows = array();
  $i = 0;
  while ($province = $result->fetchObject()) {
    $row = array();
    foreach ($province as $col => $value) {
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


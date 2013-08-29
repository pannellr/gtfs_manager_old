<?php

class StopController extends DrupalDefaultEntityController {

  /**
   * create empty stop object
   *
   * @param $sid the stop id, may be used if on update
   *
   * @return the empty Stop object
   */
  public function create($sid = '') {
    return (object) array(
      'sid' => $sid,
      'stop_name' => '',
      'stop_desc' => '',
      'stop_lat' => '',
      'stop_long' => '',
      'wheelchair_boarding' => '',
      'zone_id' => '',
      'province_id' => '',
      'time_zone_id' => '',
      'created_at' => '',
      'updated_at' => '',
      'created_by' => '',
      'updated_by' => '',
      //other table properties
      'service_id' => '',
      'arrival_time' => '',
      'departure_time' => '',
      'stop_sequence' => '',
      'shape_dist_traveled' => '',
      'start_date' => time(),
      'end_date' => time(),
      'days' => drupal_map_assoc(array(
		      t('monday'),
		      t('tuesday'),
		      t('wednesday'),
		      t('thursday'),
		      t('friday'),
		      t('saturday'),
		      t('sunday')
				       )),
      'monday' => '',
      'tuesday' => '',
      'wednesday' => '',
      'thursday' => '',
      'friday' => '',
      'saturday' => '',
      'sunday' => '',
    );
  }

  public function save($stop) {
    $transaction = db_transaction();

    try {
      global $user;

      //determine if stop is new
      $stop->is_new = empty($stop->sid);

      //set timestamps
      if (empty($stop->created_at)) {
	$stop->created_at = REQUEST_TIME;
      }

      $stop->updated_at = REQUEST_TIME;

      // give other modules a chance to prepare field data
      field_attach_presave('stop', $stop);

      // if this is a new stop
      if ($stop->is_new) {
	// Save the stop
	drupal_write_record('stops', $stop);
	$op = 'insert';
      } else {
	// update the stop
	drupal_write_record('stops', $stop, 'sid');
	$op = 'update';
      }

      // Save fields
      $function = 'field_attach_' . $op;
      $function('stop', $stop);

      module_invoke_all('entity_' . $op, $stop, 'stop');

      // Clear internal properties
      unset($stop->is_new);

      // Ignore slave server temporarily to give time for insert/update
      db_ignore_slave();

      return $stop;

      //catch exceptions and rollback database changes
    } catch (Exception $e) {
      $transaction->rollback();
      watchdog_exception('stop', $e, NULL, WATCHDOG_ERROR);
      return FALSE;
    }
  }

  public function delete($sids) {
    if (!empty($sids)) {

      $transaction = db_transaction();

      try {

	foreach ($sids as $sid) {
	  db_delete('stops')
	    ->condition('sid', $sid, '=')
	    ->execute();
	}

	db_ignore_slave();

      } catch (Exception $e) {
	$transaction->rollback();
	watchdog_exception('stop', $e, NULL, WATCHDOG_ERROR);
	return FALSE;
      }

      //Clear the page, block, and stop caches
      cache_clear_all();
      $this->resetCache();
    }
    return TRUE;
  }

  public function getStops($sids, $conditions = array()) {
    $stops = array();
    $where = '';
    $where_args = array();

    if (!empty($sids)) {
      if (sizeof($sids) > 1) {
	$where = 's.sid in :sids';
	$where_args = array(':sids' => '(' . join(',', $sids) . ')');
      } else {
	$where = 's.sid = :sid';
	$where_args = array(':sid' => $sids[0]);
      }
    } else {
      $where = 't.rid = :rid';
      $where_args = array(':rid' => $conditions[1]);
    }

    $query = 'select s.sid '
      .  ' ,  s.stop_name '
      .  ' ,  s.stop_desc '
      .  ' ,  s.stop_lat '
      .  ' ,  s.stop_long '
      .  ' ,  s.zone_id '
      .  ' ,  s.wheelchair_boarding '
      .  ' ,  s.province_id '
      .  ' ,  s.time_zone_id '
      .  ' ,  st.arrival_time '
      .  ' ,  st.departure_time '
      .  ' ,  st.stop_sequence '
      .  ' ,  st.shape_dist_traveled '
      .  ' ,  t.tid '
      .  ' ,  t.rid '
      .  ' ,  t.service_id '
      .  ' ,  c.start_date '
      .  ' ,  c.end_date '
      .  ' ,  c.monday '
      .  ' ,  c.tuesday '
      .  ' ,  c.wednesday '
      .  ' ,  c.thursday '
      .  ' ,  c.friday '
      .  ' ,  c.saturday '
      .  ' ,  c.sunday '
      .  ' ,  p.name as province_name '
      .  ' ,  p.code as province_code '
      .  ' ,  tz.name as time_zone_name '
      .  ' ,  tz.offset as tz_offset '
      .  ' ,  co.country_code '
      .  ' ,  du.distance_unit_code '
      .  ' from '
      .  ' {trips} t '
      .  ' join {stop_times} st on st.tid = t.tid '
      .  ' join {stops} s on s.sid = st.sid '
      .  ' join {calendar} c on c.service_id = t.service_id '
      .  ' join {provinces} p on p.province_id = s.province_id '
      .  ' join {countries} co on co.country_id = p.country_id '
      .  ' join {distance_units} du on du.distance_unit_id = co.distance_unit_id '
      .  ' join {time_zones} tz on tz.time_zone_id = s.time_zone_id '
      .  ' where '
      . $where
      . ' order by st.stop_sequence ';



    $result =
      db_query($query, $where_args);

    if ($result) {
      while ($stop = $result->fetchObject()) {
    	$days = array();
    	if ($stop->monday == 1) { $days['monday'] = 'monday'; }
    	if ($stop->tuesday == 1) { $days['tuesday'] = 'tuesday'; }
    	if ($stop->wednesday == 1) { $days['wednesday'] = 'wednesday'; }
    	if ($stop->thursday == 1) { $days['thursday'] = 'thursday'; }
    	if ($stop->friday == 1) { $days['friday'] = 'friday'; }
    	if ($stop->saturday == 1) { $days['saturday'] = 'saturday'; }
    	if ($stop->sunday == 1) { $days['sunday'] = 'sunday'; }
    	$stop->days = $days;

        //convert to miles if miles are the preferred distance
        if ($stop->distance_unit_code != 'km') {
          $stop->shape_dist_travelled = ($stop->shape_dist_travelled * 0.62137);
        }
	    $stops[] = $stop;
      }
    }
    return $stops;
  }

}
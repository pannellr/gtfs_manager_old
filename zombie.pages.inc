<?php

function gtfs_manager_zombie_report() {
  $query =<<<SQL
select t.tid
  ,  s.sid
  ,  s.stop_name
  ,  s.stop_desc
  ,  c.service_id
  ,  st.arrival_time
  ,  st.departure_time
  ,  c.start_date
  ,  c.end_date
from trips t
left join stop_times st
on st.tid = t.tid
left join stops s
on s.sid = st.sid
left join calendar c
on c.service_id = t.service_id
where t.rid not in (select rid from routes)
SQL;

  $results = db_query($query);
  
  $zombies = array();

  while ($zombie = $results->fetchObject()) {
    array_push($zombies, $zombie);
  }
  
  $variables['zombies'] = $zombies;

  return theme('gtfs_manager_zombie_report', $variables);

}
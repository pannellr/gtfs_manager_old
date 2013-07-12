<?php
/**
 * @file zombie report template
 */
?>
<h2>Listing zombie stops</h2>
<table class="zombie-report-table">
   <thead>
     <th>Stop Id</th>
     <th>Trip Id</th>
     <th>Calendar Id</th>
     <th>Arrival Time</th>
     <th>Departure Time</th>
     <th>Start Date</th>
     <th>End Date</th>
     <th>Remove</th>
   </thead>
   <tbody>
   <?php foreach ($zombies as $zombie) { ?>
     <tr>
       <td><?php print $zombie->sid; ?></td>
       <td><?php print $zombie->tid; ?></td>
       <td><?php print $zombie->service_id; ?></td>
       <td><?php print $zombie->arrival_time; ?></td>
       <td><?php print $zombie->departure_time; ?></td>
       <td><?php print $zombie->start_date; ?></td>
       <td><?php print $zombie->end_date; ?></td>
       <td><?php print $zombie->remove_link; ?></td>
     </tr>
   <?php } ?>
    <tr>

</div>
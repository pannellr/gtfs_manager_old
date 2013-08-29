<?php
/**
 * @file template for route stops
 *
 *
 */
?>
 <?php print implode(' | ', $variables['links']); ?>
<table class="route-stops-view">
  <thead>
    <tr>
      <th>Name</th>
      <th>Desc</th>
      <th>Province</th>
      <th>TZ</th>
      <th>Lat</th>
      <th>Long</th>
      <th>Arrives</th>
      <th>Departs</th>
      <th>Sequence</th>
      <th>Distance</th>
      <th>Calendar</th>
      <th>Features</th>
      <th>Options</th>
    </tr>
  </thead>
  <tbody>

<?php foreach ($variables['stops'] as $stop) { ?>
  <tr class="stop-row <?php print $stop->sid; ?>">
    <td><?php print $stop->stop_name; ?></td>
    <td><?php print $stop->stop_desc; ?></td>
    <td><?php print $stop->province_name; ?></td>
    <td><?php print $stop->time_zone_name; ?></td>
    <td><?php print $stop->stop_lat; ?></td>
    <td><?php print $stop->stop_long; ?></td>
    <td><?php print $stop->arrival_time; ?></td>
    <td><?php print $stop->departure_time; ?></td>
    <td><?php print $stop->stop_sequence; ?></td>
    <td><?php print $stop->shape_dist_traveled . $stop->distance_unit_code; ?></td>
    <td><?php print $stop->calendar; ?></td>
    <td><?php print $stop->features; ?></td>
    <td><?php print implode('<br />', $stop->links); ?></td>
  </tr>
<?php } ?>
 </tbody>
</table>

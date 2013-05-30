<?php
/**
 * @file search results page template
 */
?>
<div class="<?php print $classes; ?>">
  <h2><?php print $from; ?> to <?php print $to; ?> - <?php print $date; ?></h2>
  <table>
    <thead>
      <tr>
        <th></th>
	<th>Leaves</th>
	<th>Arrives</th>
	<th>Duration</th>
	<th>Trans</th>
	<th>Features</th>
	<th>Type</th>
	<th>Provider</th>
      </tr>
    </thead>
    <tbody>
      <?php $i = 1; ?>
      <?php foreach ($results as $result) { ?>
      <tr>
        <td><?php print $i; ?></td>
	<td><?php print $result->departure_departure_time; ?> - <?php print $result->departure_description; ?></td>
	<td><?php print $result->destination_arrival_time; ?> - <?php print $result->destination_description; ?></td>
	<td><?php print $result->duration; ?></td>
	<td><?php print $result->transfer;?></td>
	<td><?php print $result->wheelchair_access; ?></td>
	<td><?php print $result->transportation_type; ?></td>
	<td><?php print $result->provider; ?></td>
      </tr>
      <?php $i++; ?>
      <?php } ?>
    </tbody>
  </table>

      <?php if (!empty($return_results)) { ?>
  <h1>Return Trip</h1>
  <h2><?php print $return_from; ?> to <?php print $return_to; ?> - <?php print $return_date; ?></h2>
  <table>
    <thead>
      <tr>
        <th></th>
	<th>Leaves</th>
	<th>Arrives</th>
	<th>Duration</th>
	<th>Trans</th>
	<th>Features</th>
	<th>Type</th>
	<th>Provider</th>
      </tr>
    </thead>
    <tbody>
      <?php $i = 1; ?>
   <?php foreach ($return_results as $result) { ?>
      <tr>
        <td><?php print $i; ?></td>
	<td><?php print $result->departure_departure_time; ?> - <?php print $result->departure_description; ?></td>
	<td><?php print $result->destination_arrival_time; ?> - <?php print $result->destination_description; ?></td>
	<td><?php print $result->duration; ?></td>
	<td><?php print $result->transfer;?></td>
	<td><?php print $result->wheelchair_access; ?></td>
	<td><?php print $result->transportation_type; ?></td>
	<td><?php print $result->provider; ?></td>
      </tr>
      <?php $i++; ?>
      <?php } ?>
    </tbody>
  </table>  

      <?php } ?>

</div>
<br />
<hr>
<br />


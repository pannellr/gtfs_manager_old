<?php
/**
 * @file search results page template
 */
?>
<div class="<?php print $classes; ?>">
  <h2><?php print $from; ?> to <?php print $to; ?> - <?php print $date; ?></h2>
  <p><small><em>Fare prices are subject to chage.</em></small></p>
  <a href="#" class="change-date prev">Previous day</a> |
  <a href="#" class="change-date next">Next day</a>
  <table  class="search-results-table">
    <thead>
      <tr>
	<th>Leaves</th>
	<th>Arrives</th>
	<th>Duration</th>
	<th>$</th>
	<th>Transfers</th>
	<th>Features</th>
	<th>Type</th>
	<th>Provider</th>
        <th></th>
      </tr>
    </thead>
    <tbody>
      <?php $i = 1; ?>
      <?php foreach ($results as $result) { ?>
      <tr class="search-result-row">
	<td class="search-result-cell"><?php print $result->departure_departure_time; ?></td>
	<td class="search-result-cell"><?php print $result->destination_arrival_time; ?></td>
	<td class="search-result-cell"><?php print $result->duration . $result->days; ?></td>
	<td class="search-result-cell"><?php print $result->price; ?></td>
	<td class="search-result-cell"><?php print $result->transfer;?></td>
	<td class="search-result-cell"><?php print $result->features;?></td>
	<td class="search-result-cell"><?php print $result->transportation_type; ?></td>
	<td class="search-result-cell"><?php print $result->provider; ?></td>
	<td class="search-result-cell"><span class="collapsitablesection" id="collapsitable<?php print $i; ?>">More Details</span></td>
      </tr>
	 <tr id="collapsitable<?php print $i; ?>" class="collapsitable">
           <td colspan="9"><?php print $result->more_info; ?></td>
         </tr>
      <?php $i++; ?>
      <?php } ?>
    </tbody>
  </table>


      <?php if (!empty($return_results)) { ?>
  <h1>Return Trip</h1>
  <h2><?php print $to; ?> to <?php print $from; ?> - <?php print $return_date; ?></h2>
  <a href="#" class="change-date prev return">Previous day</a> |
  <a href="#" class="change-date next return">Next day</a>
  <table>
    <thead>
      <tr>
	<th>Leaves</th>
	<th>Arrives</th>
	<th>Duration</th>
	<th>$</th>
	<th>Transfers</th>
	<th>Features</th>
	<th>Type</th>
	<th>Provider</th>
        <th></th>
      </tr>
    </thead>
    <tbody>
      <?php $i = 1; ?>
   <?php foreach ($return_results as $result) { ?>
      <tr>
	<td><?php print $result->departure_departure_time; ?></td>
	<td><?php print $result->destination_arrival_time; ?></td>
	<td><?php print $result->duration; ?></td>
	<td><?php print $result->price; ?></td>
	<td><?php print $result->transfer;?></td>
	<td><?php print $result->features;?></td>
	<td><?php print $result->transportation_type; ?></td>
	<td><?php print $result->provider; ?></td>
        <td>
          <span class="collapsitablesection" id="return-collapsitable<?php print $i; ?>">More Details</span>
        </td>
      </tr>
        <tr id="return-collapsitable<?php print $i; ?>" class="collapsitable">
          <td colspan="9"><?php print $result->more_info; ?></td>
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


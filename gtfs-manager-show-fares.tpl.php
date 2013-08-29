<?php
/**
 * @file show fares page template
 */
?>

<div class="<?php print $classes; ?>">
  <h2>Showing all fares</h2>
  <table class="all-fares-table">
    <thead>
      <tr>
	<th>Price</th>
	<th>Currency Type</th>
	<th>Payment method</th>
	<th></th>
      </tr>
    </thead>
    <tbody>

      <?php foreach ($fares as $fare) { ?>
      
      <tr>
	<td><?php print money_format('$%i', $fare->price); ?></td>
	<td><?php print $fare->currency_type_code; ?></td>
	<td>
	  <?php 
	     print $fare->payment_method == 0 ? 'Pay on board' : 'Pay before boarding'; 
	  ?>
	</td>
	<td><?php print $fare->edit_link; ?></td>
       </tr>
       <?php } ?>
      
     </tbody>
   </table>
</div>

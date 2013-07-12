(function ($) {
Drupal.behaviors.togg = {
  attach: function (context, settings) {
  $('span.collapsitablesection').each(function() {
    $(this).html('<a class="collapsitablesection" href="#">' + $(this).html() + '</a>');
  });

  $('a.collapsitablesection').each(function () {
    $(this).click(function(e) {
      $("tr#" + $(this).closest('span').attr('id')).toggle(200);
      return false;
    });
  });
}
};

Drupal.behaviors.nextPrev = {
  attach: function (context, settings) {
    $('.change-date').click(function(e) {
	e.preventDefault();
	
	var direction = ($(this).hasClass('prev')) ? 'prev' : 'next';

	var element = ($(this).hasClass('return')) ? '.return-date' : '.search-date';

	changeSearchDate(element, direction, 1);
	$(element).closest('form').submit();
    });


      function changeSearchDate (element, direction, changeValue) {
	  var dateStrings = $(element).val().split('-'); 
	  var travelDate = new Date(dateStrings[2],dateStrings[1],dateStrings[0]);
	  if (direction === 'prev') {
	      travelDate.setDate(travelDate.getDate() - changeValue);
	  } else {
	      travelDate.setDate(travelDate.getDate() + changeValue);
	  }
	  var newDateString = travelDate.getDate() + '-'
	    + travelDate.getMonth() + '-'
	    + travelDate.getFullYear();

	$(element).effect( "highlight",{ color: "green" }, 800);
	$(element).val(newDateString);
      }

  }};

})(jQuery);

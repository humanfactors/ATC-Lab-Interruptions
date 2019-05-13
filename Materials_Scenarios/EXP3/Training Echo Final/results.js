$(document).ready(function () {
        $(".A").change(function () {
            if ($(this).is(":checked"))
                $(this).parent().parent().parent().next(".textbox").show();
            else
                $(this).parent().parent().parent().next(".textbox").hide();
        });
		$(".B").change(function () {
            if ($(this).is(":checked"))
                $(this).parent().parent().parent().next(".textbox").show();
            else
                $(this).parent().parent().parent().next(".textbox").hide();
        });
		$(".C").change(function () {
            if ($(this).is(":checked"))
                $(this).parent().parent().parent().next(".textbox").show();
            else
                $(this).parent().parent().parent().next(".textbox").hide();
        });
    });
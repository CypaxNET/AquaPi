(function($) {
    $.fn.weekly_schedule = function(callerSettings) {

        var settings = $.extend({
            days: ["Mo", "Tu", "We", "Th", "Fr", "Sa", "Su"], // Days displayed
            hours: "7:00AM-10:00PM", // Hours displyed
            fontFamily: "Montserrat", // Font used in the component
            fontColor: "black", // Font colot used in the component
            fontWeight: "100", // Font weight used in the component
            fontSize: "0.8em", // Font size used in the component
            hoverColor: "#727bad", // Background color when hovered
            selectionColor: "#9aa7ee", // Background color when selected
            headerBackgroundColor: "transparent", // Background color of headers
            onSelected: function(){}, // handler called after selection
            onRemoved: function(){}, // handler called after removal
            selection: ""
        }, callerSettings||{});

        settings.hoursParsed = parseHours(settings.hours);

        var mousedown = false;
        var devarionMode = false;

        var schedule = this;

        function getSelectedHour() {
            var days = settings.days;
            var dayContainer = schedule.find(".day");
            var output = "";
            for (var i = 0; i < dayContainer.length; i++) {
                var children = $(dayContainer[i]).children();
                for (var j = 0; j < children.length; j++) {
                    if ($(children[j]).hasClass('selected')) {
                      output = output + "1";
                    } else {
                      output = output + "0";
                    }
                }
                if (i < (dayContainer.length-1)) output = output + ",";
            }
            return output;
        }

        function setSelectedHour(dataString) {
            var split = dataString.split(",");
            var days = settings.days;
            var dayContainer = schedule.find(".day");
            var splitLength = split.length;
            for (var i = 0; i < dayContainer.length; i++) {
                var children = $(dayContainer[i]).children();
                var hourCount = 0;
                if (i < splitLength) hourCount = split[i].length;
                for (var j = 0; j < children.length; j++) {
                    var newValue = "0";
                    if ((i < splitLength) && (j < hourCount)) newValue = split[i].charAt(j);
                    if ($(children[j]).hasClass('selected')) {
                      if (newValue == "0") {
                        $(children[j]).removeClass('selected');
                      }
                    } else {
                      if (newValue == "1") {
                        $(children[j]).addClass('selected');
                      }
                    }
                }
            }
            return 0;
        }


        if (typeof callerSettings == 'string') {
            switch (callerSettings) {
                case 'getSelectedHour':
                    return getSelectedHour();
                    break;
                default:
                    if (callerSettings.startsWith('setSelectedHour')) {
                      return setSelectedHour(callerSettings.replace('setSelectedHour', ''));
                    } else {
                      throw 'Weekly schedule method unrecognized!'
                    }
            }
        }
        // options is an object, initialize!
        else {
            var days = settings.days; // option
            var hours = settings.hoursParsed; // option

            $(schedule).addClass('schedule');

            /*
             * Generate Necessary DOMs
             */

            // Create Header
            var dayHeaderContainer = $('<div></div>', {
                class: "header"
            });

            var align_block = $('<div></div>', {
                class: "align-block"
            });

            dayHeaderContainer.append(align_block);

            // Insert header items
            for (var i = 0; i < days.length; ++i) {
                var day_header = $('<div></div>', {
                    class: "text-005"+ i + " header-item " + days[i] + "-header"
                });
                var header_title = $('<h3>' + capitalize(days[i]) + '</h3>')

                day_header.append(header_title);
                dayHeaderContainer.append(day_header);
            }


            var days_wrapper = $('<div></div>', {
                class: 'days-wrapper'
            });

            var hourHeaderContainer = $('<div></div>', {
                class: 'hour-header'
            });

            days_wrapper.append(hourHeaderContainer);

            for (var i = 0; i < hours.length; i++) {
                var hour_header_item = $('<div></div>', {
                    class: 'hour-header-item ' + hours[i]
                });

                var header_title = $('<h5>' + hours[i] +'</h5>');

                hour_header_item.append(header_title);
                hourHeaderContainer.append(hour_header_item);
            }


            var selection = settings.selection.split(",");

            //console.log(selection[0].length);

            for(var i = 0; i < days.length; i++) {
                var day = $('<div></div>', {
                    class: "day " + days[i]
                });

                for(var j = 0; j < hours.length; j++) {
                    
                    if ((selection.length > i) && (selection[i].length > j) && (selection[i].charAt(j) == "1")) {
                      var hour = $('<div></div>', {
                          class: "hour " + hours[j] + " selected"
                      });
                    } else {
                      var hour = $('<div></div>', {
                          class: "hour " + hours[j]
                      });
                    }

                    day.append(hour);
                }

                days_wrapper.append(day);
            }

            /*
             * Inject objects
             */

            $(schedule).append(dayHeaderContainer);
            $(schedule).append(days_wrapper);


            /*
             *  Style Everything
             */
            $('.schedule').css({
                width: "100%",
                display: "flex",
                flexDirection: "column",
                justifyContent: "flex-start"
            });

            $('.header').css({
                height: "30px",
                width: "100%",
                background: settings.headerBackgroundColor,
                marginBottom: "5px",
                display: "flex",
                flexDirection: "row"
            });
            $('.align-block').css({
                width: "100%",
                height: "100%",
                background: settings.headerBackgroundColor,
                margin: "3px"
            });

            // Style Header Items
            $('.header-item').css({
                width: '100%',
                height: '100%',
                margin: '2px' // option
            });
            $('.header-item h3').css({
                margin: 0,
                textAlign: 'center',
                verticalAlign: 'middle',
                position: "relative",
                top: "50%",
                color: settings.fontColor,
                fontFamily: settings.fontFamily,
                fontSize: settings.fontSize,
                fontWeight: settings.fontWeight,
                transform: "translateY(-50%)",
                userSelect: "none"
            });

            $('.hour-header').css({
                display: 'flex',
                flexDirection: 'column',
                margin: '2px', // option
                marginRight: '1px',
                background: settings.headerBackgroundColor,
                width: '100%'
            });

            $('.days-wrapper').css({
                width: "100%",
                height: "100%",
                background: "transparent", //option
                display: "flex",
                flexDirection: "row",
                justifyContent: "flex-start",
                position: "relative"
            });

            $('.hour-header-item').css({
                width: "100%",
                height: "100%",
                border: "none", // option
                borderColor: "none", // option
                borderStyle: "none" // option
            });
            $('.hour-header-item h5').css({
                color: settings.fontColor,
                margin: "0", // option
                textAlign: "right",
                verticalAlign: "middle",
                position: "relative",
                fontFamily: settings.fontFamily,
                fontSize: settings.fontSize,
                fontWeight: settings.fontWeight,
                paddingRight: "10%",
                userSelect: "none"
            });

            $('.day').css({
                display: "flex",
                flexDirection: "column",
                marginRight: "1px", // option
                marginBottom: "1px",
                background: "transparent", // option
                width: "100%"
            });
            $('.hour').css({
                background: "#dddddd", // option
                marginBottom: "1px", // option
                width: "100%",
                height: "100%",
                userSelect: "none"
            });

            /*
             * Hook eventlisteners
             */

            $("<style type='text/css' scoped> .hover { background: "
                + settings.hoverColor +
                " !important;} .selected { background: "
                + settings.selectionColor +
                " !important; } .disabled { pointer-events: none !important; opacity: 0.3 !important; box-shadow: none !important; }</style>")
                .appendTo(schedule);

            // Prevent Right Click
            $('.schedule').on('contextmenu', function() {
                return false;
            });

            $('.hour').on('mouseenter', function() {
                if (!mousedown) {
                    $(this).addClass('hover');
                }
                else {
                    if (devarionMode) {
                        $(this).removeClass('selected');
                    }
                    else {
                        $(this).addClass('selected');
                    }
                }
            }).on('mousedown', function() {
                mousedown = true;
                focusOn($(this).parent());

                if ($(this).hasClass('selected')) {
                    $(this).removeClass('selected');
                    schedule.trigger('selectionremoved')
                    devarionMode = true;
                }
                else {
                    $(this).addClass('selected');
                    schedule.trigger('selectionmade')
                }
                $(this).removeClass('hover');
            }).on('mouseup', function() {
                devarionMode = false;
                mousedown = false;
                clearFocus();
								schedule.trigger('selectionmade')
            }).on('mouseleave', function () {
                if (!mousedown) {
                    $(this).removeClass('hover');
                }
            });

        }

        function parseHours(string) {
            var output = [];

            var split = string.toUpperCase().split("-");
            var startInt = parseInt(split[0].split(":")[0]);
            var endInt = parseInt(split[1].split(":")[0]);

            var startHour = split[0].includes("PM") ? startInt + 12 : startInt;
            var endHour = split[1].includes("PM") ? endInt + 12 : endInt;

            var curHour = startHour;

            for (curHour; curHour <= endHour; curHour++) {
                var parsedStr = "";

                //parsedStr += (curHour).toString() + ":00&nbsp;-&nbsp;" + (curHour+1).toString() + ":00";
                parsedStr += (curHour).toString() + ":00";

                output.push(parsedStr);
            }

            return output;
        }

        function capitalize(string) {
            return string.charAt(0).toUpperCase() + string.slice(1);
        }

        function focusOn(day) {
            var targetDayClass = $(day).attr('class').split('\ ')[1];
            var dayContainer = schedule.find(".day");

            for (var i = 0; i < dayContainer.length; i++) {
                if ($(dayContainer[i]).hasClass(targetDayClass)) {
                    continue;
                }

                var hours = $(dayContainer[i]).children();
                for (var j = 0; j < hours.length; j++) {
                    $(hours[j]).addClass('disabled');
                }
            }

            $(day).on('mouseleave', function() {
                clearFocus();
                mousedown = false;
                devarionMode = false;
            });
        }

        function clearFocus() {
            var dayContainer = schedule.find(".day");

            for (var i = 0; i < dayContainer.length; i++) {

                var hours = $(dayContainer[i]).children();
                for (var j = 0; j < hours.length; j++) {
                    $(hours[j]).removeClass('disabled');
                }
            }
        }

    };
}(jQuery));

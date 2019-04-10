/*!
* File identification:			$Id: aquarium.js 50 2017-12-13 16:35:37Z cypax $
* Revision of last commit:	$Rev: 50 $
* Author of last commit:		$Author: cypax $
* Date of last commit:			$Date: 2017-12-13 17:35:37 +0100 (Mi, 13 Dez 2017) $
* @file aquarium.js
* @brief Aquarium web application custom JavaScript.
* @author Philipp Bank
*/

/******************************************************************************
* @brief This function loads the XML file which contains the language 
* strings, searches all translation tags for the requested language and
* the content of all classes with a corresponding transition ID with the
* respective translation text.
******************************************************************************/
function setLanguage(language) {
	console.log('setLanguage with '+language);
	language = language.replace(' ', '_');

	$.cookie("language", language, { path: '/', expires: 365 });

	// In order to get the translations,
	// we must use Ajax to load the XML
	// file and replace the contents
	// of the elements that need translating
	$.ajax({
			// Here, we specify the file that
			// contains our translations
			url: 'languages.xml',
			// The following code is run when
			// the file is successfully read
			success: function(xml) {
				// jQuery will find all <translation>
				// tags and loop through each of them
				$(xml).find('translation').each(function(){
						// We fetch the id we set in the XML
						// file and set a var 'id'
						var id = $(this).attr('id');
						// This is the most important step.
						// Based on the language we can set,
						// jQuery will search for a matching
						// tag and return the text inside it
						var text = $(this).find(language).text();
						// Last, but not least, we set the
						// contents of the elements with a
						// class name matching the id in the
						// XML file to the text we just
						// fetched
						$("." + id).html(text);
						
						/*
						if(id == "text-0025")
						{
							$("#soundswitch").flipswitch("option", "offText", text);
						}
						if(id == "text-0009")
						{
							$("#soundswitch").flipswitch("option", "onText", text);
						}
						*/
						
				});
			}
	});
};
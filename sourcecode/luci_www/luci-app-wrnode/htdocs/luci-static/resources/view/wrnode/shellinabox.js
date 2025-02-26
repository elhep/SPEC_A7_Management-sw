// 'use strict';
'require view';

return view.extend({
	handleSaveApply: null,
	handleSave: null,
	handleReset: null,

	load: function () {
		var shellinabox = 'http://' + window.location.host + ':4200/'
		location.replace(shellinabox)
	}
});
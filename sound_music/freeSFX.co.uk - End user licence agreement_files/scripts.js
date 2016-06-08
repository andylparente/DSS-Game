Event.observe(window, 'load', function() {
	var n = new popout($('popout'));	
});


var playlink = Class.create({
	initialize: function(link) {

		this.options = Object.extend({
			   'url': '/www/ajax/preview.php'
			  }, arguments[1] || { });

		this.link = link;
        this.link.observe('click', this.click.bindAsEventListener(this));
		this.smid= this.link.href;
		var this_O = this;
		this.sm = soundManager.createSound({
			'id':this.smid,
			'url':this.link.href,
			'onfinish': function(){
					this_O.link.removeClassName('playing');
			}
		});
	},

	click: function(ev) {
		ev.stop();
		if (this.link.hasClassName('playing')) {
			this.pause();
		} else if(this.link.hasClassName('paused')) {
			this.resume();
		}else {
			this.play();
		}
	},

	play: function() {
		if(this.link.id) {
		new Ajax.Request(this.options.url, {
			  method: 'post',
			  parameters: {'sid':this.link.id},
			  onSuccess: function(transport) {								
			  }
			});
		}
		this.resetAll();
		this.sm.play();
		this.link.removeClassName('paused');
		this.link.addClassName('playing');
	},

	pause: function() {
		this.sm.pause();
		this.link.removeClassName('playing');
		this.link.addClassName('paused');
	},

	resume: function() {
		this.sm.resume();
		this.link.removeClassName('paused');
		this.link.addClassName('playing');
	},

	resetAll: function(ev) {
		soundManager.stopAll();
		$$('.player').each(function(el) {
					el.removeClassName('playing');
					el.removeClassName('paused');
		});

	}

});



var popout = Class.create({
	initialize: function(stage) {
		this.options = Object.extend({
			   'popOut': 'popout',
			   'adBox ': 'adbox'
			  }, arguments[1] || { });

		this.stage = stage;
        this.stage.select('.open')[0].observe('click', this.tOpen.bindAsEventListener(this));
		this.stage.select('.close')[0].observe('click', this.tClose.bindAsEventListener(this));
		var this_O = this;
		var status = 0;
		
	},

	tOpen: function(ev) {
		ev.stop();
		if (status == 0){
			this.stage.setStyle({'width':'325px'});
			new Effect.Move($('adbox'), { x: 300, y: 0, mode: 'relative' });
			status = 1;
			pageTracker._trackEvent('Pop out Ad', 'click', 'open');
		} else {
			this.tClose();
		}
		
	},

	tClose: function(ev) {
		var this_O = this;
		ev.stop();
		new Effect.Move($('adbox'), { x: -300, y: 0, mode: 'relative', 
						  			afterFinish:function(){	this_O.stage.setStyle({'width':'20px'});} });
	
		status = 0;
		pageTracker._trackEvent('Pop out Ad', 'click', 'close');
	}

});






var editupload = Class.create({
	initialize: function(div) {

		this.options = Object.extend({
			   'url': ''
			  }, arguments[1] || { });

		this.div = div;
		this.id =  this.div.readAttribute('sid');
		this.cmddelete = $('delete_' + this.id );
		this.cmdsave =  $('save_' + this.id );
		this.txttitle = $('title_' + this.id );
		this.txttags = $('tags_' + this.id );
		this.txttitleerror = $('titleerror_' + this.id );
		this.txttagserror = $('tagserror_' + this.id );

        this.cmddelete.observe('click', this.del.bindAsEventListener(this));
    	this.cmdsave.observe('click', this.save.bindAsEventListener(this));

	},

	del: function(ev) {
			ev.stop();
			var thisO = this;
			new Ajax.Request(this.options.url,{
				'parameters':{ 'id':this.id,
							  'action':'delete'},
				onSuccess: function(transport) {
					if (transport.responseText == '1'){
						Effect.Fade(thisO.div);
					} else {
						var errJSON = transport.responseText.evalJSON(true);
						if (errJSON.general) thisO.txttitleerror.innerHTML = errJSON.general;
						//ERROR!
					}
				}
		});
},

	save: function(ev) {
			ev.stop();
			var thisO = this;
			new Ajax.Request(this.options.url,{
				'parameters':{ 'id':this.id,
							   'action':'update',
							   'title':$F(this.txttitle),
							   'tags':$F(this.txttags)},
				onSuccess: function(transport) {
					if (transport.responseText == '1'){
						Effect.Fade(thisO.div);
					} else {
						var errJSON = transport.responseText.evalJSON(true);
						if (errJSON.title) thisO.txttitleerror.innerHTML = errJSON.title;
						if (errJSON.tags) thisO.txttagserror.innerHTML = errJSON.tags;
						//ERROR!
					}
				}
			});

			//
	}
});
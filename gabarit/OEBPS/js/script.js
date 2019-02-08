/* Script jeu 25th May*/
var drapeau;
var reponse;

//function isPlaying(audelem) { return !audelem.paused; }


function checkAnswer(flag) {
    drapeau = flag;
    
    document.getElementById('modal').style.visibility='visible';
}

function compareAnswer(answer) {
        reponse = answer;
        if(drapeau == reponse)
            {
                var right = document.getElementById('right');
                right.play();
                document.getElementById('modal').style.visibility='hidden';
                document.getElementById(drapeau + 'Name').style.visibility='visible';
                var flagSound = drapeau+"Sound";
                var sound = document.getElementById(flagSound);
                setTimeout(function(){sound.play()}, 1000);
            }
        else
            {
                var wrong = document.getElementById('wrong');
                wrong.play();
            }
    }

var cible = 'done';
var choix;

function selectTarget(target){
    if (cible != 'done')
        {
            document.getElementById(cible).style.backgroundColor='white';
        }
    cible = target;
    document.getElementById(cible).style.backgroundColor='#748cc9';
    document.getElementById('choices').style.visibility='visible';
    
}

/* remplace drag and drop*/

function findWord(choice){
    choix = choice;
    document.getElementById(cible).style.backgroundColor='white';
    if(choice + 0 == cible){
        document.getElementById(cible).style.backgroundColor='#748cc9';
        var right = document.getElementById('right');
        right.play();
        document.getElementById(cible).innerHTML = document.getElementById(choice).innerHTML;
        document.getElementById('choices').removeChild(document.getElementById(choice));
        var choixSound = choix+"Sound";
        var sound = document.getElementById(choixSound);
        setTimeout(function(){sound.play()}, 1000);
        document.getElementById(cible).onclick = null;
        cible = 'done';
    }
    else
        {
            var wrong = document.getElementById('wrong');
            wrong.play();
        }
    document.getElementById('choices').style.visibility='hidden';
        
}





/* Script lecture audio*/
function JouerSon(id) {
            var voice = id;
            var sound = document.getElementById(voice);
            sound.play();
    }


/* cache le texte "réponse" et affiche la réponse */

function getRep(id) {
    document.getElementById(id + "Rep").appendChild(document.getElementById(id));
    document.getElementById(id).style.visibility='hidden';
    document.getElementById(id + "Rep").style.visibility='visible';

}

/* Joue le son associé à la réponse et l'affiche */

function getRepAndSound(id) {
    var voice = id + "Sound";
    var sound = document.getElementById(voice);
    sound.play();
    document.getElementById(id + "Rep").appendChild(document.getElementById(id));
    document.getElementById(id).style.visibility='hidden';
    document.getElementById(id + "Rep").style.visibility='visible';

}


/* Fait descendre la page de 1536px */
function scrollwin(){
    window.scrollBy(0, 1536);
}

/* Affiche une réponse complète quand bonne proposition choisie */

function blocQuestion(rep, qId){
    if(rep == 'right'){
        var right = document.getElementById('right');
        var toPlay = qId + "Sound";
        var sound = document.getElementById(toPlay);
            sound.play();
            setTimeout(function(){right.play()}, 1200);
        
        document.getElementById(qId + "Rep").appendChild(document.getElementById(qId));
        document.getElementById(qId + "Rep").removeChild(document.getElementById(qId));
        document.getElementById(qId + "Rep").style.visibility='visible';
        document.getElementById(qId + "Rep").style.width='40%';
        document.getElementById(qId + "Rep").style.heigh='auto';
        document.getElementById(qId + "Rep").style.position='relative';
    }
    else if(rep == 'nogreat'){
        var toPlay = qId + "Sound";
        var sound = document.getElementById(toPlay);
            setTimeout(function(){sound.play()}, 700);
        document.getElementById(qId + "Rep").appendChild(document.getElementById(qId));
        document.getElementById(qId + "Rep").removeChild(document.getElementById(qId));
        document.getElementById(qId + "Rep").style.visibility='visible';
        document.getElementById(qId + "Rep").style.width='40%';
        document.getElementById(qId + "Rep").style.heigh='auto';
        document.getElementById(qId + "Rep").style.position='relative';
    }
    else{
        var wrong = document.getElementById('wrong');
        var sound = document.getElementById(qId);
        sound.play();
        setTimeout(function(){wrong.play()}, 1200);
        
    }
}

/* Affiche une réponse complète quand bonne proposition choisie - délai adapté aux longues réponses*/

function blocQuestionLong(rep, qId){
    if(rep == 'right'){
        var right = document.getElementById('right');
        var toPlay = qId + "Sound";
        var sound = document.getElementById(toPlay);
            sound.play();
            setTimeout(function(){right.play()}, 2200);
        
        document.getElementById(qId + "Rep").appendChild(document.getElementById(qId));
        document.getElementById(qId + "Rep").removeChild(document.getElementById(qId));
        document.getElementById(qId + "Rep").style.visibility='visible';
        document.getElementById(qId + "Rep").style.width='40%';
        document.getElementById(qId + "Rep").style.heigh='auto';
        document.getElementById(qId + "Rep").style.position='relative';
    }
    else if(rep == 'nogreat'){
        var toPlay = qId + "Sound";
        var sound = document.getElementById(toPlay);
            setTimeout(function(){sound.play()}, 700);
        document.getElementById(qId + "Rep").appendChild(document.getElementById(qId));
        document.getElementById(qId + "Rep").removeChild(document.getElementById(qId));
        document.getElementById(qId + "Rep").style.visibility='visible';
        document.getElementById(qId + "Rep").style.width='40%';
        document.getElementById(qId + "Rep").style.heigh='auto';
        document.getElementById(qId + "Rep").style.position='relative';
    }
    else{
        var wrong = document.getElementById('wrong');
        var sound = document.getElementById(qId);
        sound.play();
        setTimeout(function(){wrong.play()}, 2200);
        
    }
}

/* Affiche réponse quand image cliquée */

function imgRep(id){
    document.getElementById(id + "Txt").appendChild(document.getElementById(id));
    document.getElementById(id).style.visibility='hidden';
    document.getElementById(id + "Txt").style.visibility='visible';
    var sound = document.getElementById(id + "Sound");
    sound.play();
}

/* Fonction map area */


/* script drag and drop non utilisé
var source;
var targetDrop;

function allowDrop(ev) {
    ev.preventDefault();
}

function drag(ev) {
    ev.dataTransfer.setData("text", ev.target.id);
}

function drop(ev) {
    ev.preventDefault();
    var data = ev.dataTransfer.getData("text");
    source = data;
    targetDrop = event.target.id;
    if (source + 0 == targetDrop){
            ev.target.appendChild(document.getElementById(data));
            var right = document.getElementById('right');
            right.play();
            var targetDropSound = source+"Sound";
            var sound = document.getElementById(targetDropSound);
            setTimeout(function(){sound.play()}, 1000);
        }
    else{
            var wrong = document.getElementById('wrong');
            wrong.play();
        }
}*/

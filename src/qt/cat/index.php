<!DOCTYPE html>
<html lang="en">
<head>
<title>Cat avatar generator</title>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<meta http-equiv="Content-Language" content="en-ca" />
<link rel="Shortcut Icon" href="favicon.png" type="image/x-icon" />

<style type="text/css" media="screen">
body { margin: 2em; padding: 0; background: #000; color: #666; font-size: .8rem; text-align: center; }
#wrapper { text-align: left; background: #fff; max-width: 300px; padding: 1rem;    margin: 0 auto; border: 1em #333 solid; text-align: center; }
h1 { font-variant: small-caps; color: hotpink; font-size: 1.8rem; font-family: Ubuntu, Arial, sans; font-weight: bold; margin: 0 0 0.3rem 0; }
.avatar { border: 0; margin: 2rem 0 0 0; }
a { color: #00aad4;}
a:hover {    color: #222;}
.smallbutton { font-size: 1rem; margin-bottom: 0.8rem; }
input[type=text]{width:90%}
.bigbutton { font-weight: bold; font-size: 1.3rem; margin-bottom: 1.3rem; }
@media (max-width: 500px) {
body { margin: 0; padding: 0; background: #FFF; font-size: 1rem; }
#wrapper { text-align: left; background: #fff; max-width: 100%; padding: 0; margin: 0 auto; border: none; text-align: center; }
h1 { font-size: 1.6rem; margin: 0.2rem 0 0.2rem 0; }
}
</style>

</head>
<body>
  
<?php
# Get var
$seed = htmlspecialchars($_GET["seed"]);
# Convert accented, thx http://stackoverflow.com/a/3373364 for the list
$unwanted = array( 'Š'=>'S', 'š'=>'s', 'Ž'=>'Z', 'ž'=>'z', 'À'=>'A', 'Á'=>'A', 'Â'=>'A', 'Ã'=>'A', 'Ä'=>'A', 'Å'=>'A', 'Æ'=>'A', 'Ç'=>'C', 'È'=>'E', 'É'=>'E',
                   'Ê'=>'E', 'Ë'=>'E', 'Ì'=>'I', 'Í'=>'I', 'Î'=>'I', 'Ï'=>'I', 'Ñ'=>'N', 'Ò'=>'O', 'Ó'=>'O', 'Ô'=>'O', 'Õ'=>'O', 'Ö'=>'O', 'Ø'=>'O', 'Ù'=>'U',
                   'Ú'=>'U', 'Û'=>'U', 'Ü'=>'U', 'Ý'=>'Y', 'Þ'=>'B', 'ß'=>'Ss', 'à'=>'a', 'á'=>'a', 'â'=>'a', 'ã'=>'a', 'ä'=>'a', 'å'=>'a', 'æ'=>'a', 'ç'=>'c',
                   'è'=>'e', 'é'=>'e', 'ê'=>'e', 'ë'=>'e', 'ì'=>'i', 'í'=>'i', 'î'=>'i', 'ï'=>'i', 'ð'=>'o', 'ñ'=>'n', 'ò'=>'o', 'ó'=>'o', 'ô'=>'o', 'õ'=>'o',
                   'ö'=>'o', 'ø'=>'o', 'ù'=>'u', 'ú'=>'u', 'û'=>'u', 'ý'=>'y', 'þ'=>'b', 'ÿ'=>'y' );
$seed = strtr( $seed, $unwanted );
# Security against code injection
$seed = preg_replace('/[^A-Za-z0-9\._-]/', '', $seed); 
# Limit the string to 35 char
$seed = substr($seed,0,35).'';
if ($seed == '') {
    $seed = uniqid();
}
# Easter egg
if ($seed == 'carrot' OR $seed == 'Carrot') {
    # waltzo is the seed to show Carrot
    $seed = 'waltzo';
}
if ($seed == 'rhel' OR $seed == 'Redhat' OR $seed == 'RHEL' OR $seed == 'redhat') {
    # waltzo is the seed to show Carrot
    $seed = '16';
}
if ($seed == 'cute' OR $seed == 'cutie' OR $seed == 'qt' OR $seed == 'Qt') {
    # waltzo is the seed to show Carrot
    $seed = '583df35cdfe3c';
}
if ($seed == 'metal' OR $seed == 'tatoo' OR $seed == 'rock' OR $seed == 'biker') {
    # waltzo is the seed to show Carrot
    $seed = '583df4b9ac346';
}
if ($seed == 'fuck' OR $seed == 'shit' OR $seed == 'ass') {
    # waltzo is the seed to show Carrot
    $seed = 'pop';
}
?>

<div id="wrapper">
    
  <h1>Cat avatar generator</h1>
  
  <img class="avatar" src="avatar.php?seed=<?php echo "$seed"; ?>" title="to download the picture, right click and SAVE AS *.PNG picture" >

  <?php 
  # workaround to display secret easter egg. 
  if ($seed == 'waltzo') {
    echo '<br/>Bravo, easter egg #1! <br/> <strong>~ Carrot ~</strong> <br/> <em>(from <a href="http://www.peppercarrot.com/">Pepper&amp;Carrot</a>)</em>';
  
  } elseif ($seed == '16') {
    echo '<br/>Bravo, easter egg #2! <br/> <strong>~ Red Hat Linux ~</strong>';
  
  } elseif ($seed == '583df35cdfe3c') {
    echo '<br/>Bravo, easter egg #3! <br/> <strong>~ Cute cat ~</strong>';
  
  } elseif ($seed == '583df4b9ac346') {
    echo '<br/>Bravo, easter egg #4! <br/> <strong>~ Rock\'n\'roll !!! ~</strong>';
  
  } elseif ($seed == 'pop') {
    echo '<br/><strong>OH! mind your words!</strong>';
  
  } else {
    #display the seed as name
    echo '<br/><strong>~ '.$seed.' ~</strong>';
  }
  ?>
  
  <form>
    <br/>
    <br/>
    Your name?*<br/>
    <input class="smallbutton" type="text" name="seed" id="name" value="" autofocus />
    <input class="bigbutton" type="submit" value="Generate new cat" />
  </form>
  
  <br/>
  <em>(* empty=random)</em><br/><br/>
  Artworks: CC-By David Revoy <a href="http://www.peppercarrot.com" title="my webcomic">[site]</a><br/>
  Code: CC-By Andreas Gohr <a href="https://www.splitbrain.org/blog/2007-01/20_monsterid_as_gravatar_fallback" title="original author">[site]</a><br/><br/>
  <a href="2016_cat-generator.zip" title="original author">[Zip sources and artwork]</a>

</div>
</body>
</html>

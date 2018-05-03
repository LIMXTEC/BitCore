<?php

build_monster($_REQUEST['seed'],$_REQUEST['size']);

function build_monster($seed='',$size=''){
    // init random seed
    if($seed) srand( hexdec(substr(md5($seed),0,6)) );

    // throw the dice for body parts
    $parts = array(
        'body' => rand(1,15),
        'fur' => rand(1,10),
        'eyes' => rand(1,15),
        'mouth' => rand(1,10),
        'accessorie' => rand(1,20)
    );

    // create backgound
    $monster = @imagecreatetruecolor(256, 256)
        or die("GD image create failed");
    $white   = imagecolorallocate($monster, 255, 255, 255);
    imagefill($monster,0,0,$white);

    // add parts
    foreach($parts as $part => $num){
        $file = dirname(__FILE__).'/img/'.$part.'_'.$num.'.png';

        $im = @imagecreatefrompng($file);
        if(!$im) die('Failed to load '.$file);
        imageSaveAlpha($im, true);
        imagecopy($monster,$im,0,0,0,0,256,256);
        imagedestroy($im);

    }

    // restore random seed
    if($seed) srand();

    // resize if needed, then output
    if($size && $size < 400){
        $out = @imagecreatetruecolor($size,$size)
            or die("GD image create failed");
        imagecopyresampled($out,$monster,0,0,0,0,$size,$size,256,256);
        header ("Content-type: image/png");
        imagepng($out);
        imagedestroy($out);
        imagedestroy($monster);
    }else{
        header ("Content-type: image/png");
        imagepng($monster);
        imagedestroy($monster);
    }
}

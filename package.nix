{ stdenv, lib, fetchFromGitHub, libX11, libXft, libXinerama, imlib2 }:

stdenv.mkDerivation {
  name = "dwm-atar13";
  version = "0-unstable-2023-04-11";


  src = fetchFromGitHub {
    owner = "atar13";
    repo = "dwm";
    rev = "83dcec7c00d88c5fd84ee1c1246e487fc2cece6e";
    sha256 = "sha256-mpWvIc8j4vDoipCputK/JdO7m3bzDCQ9M8L1dXFOK+s=";
  };

  buildInputs = [ libX11 libXft libXinerama imlib2 ];

  prePatch = ''
    substituteInPlace config.mk --replace '/usr/local' $out
  '';

  meta = with lib; {
    description = "atar13's fork of dwm";
    homepage = "https://github.com/atar13/dwm";
    license = licenses.mit;
    platforms = platforms.all;
  };
}


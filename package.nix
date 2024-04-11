{ stdenv, lib, fetchFromGitHub, libX11, libXft, libXinerama, imlib2 }:

stdenv.mkDerivation {
  name = "dwm-atar13";
  version = "0-unstable-2023-04-11";


  src = fetchFromGitHub {
    owner = "atar13";
    repo = "dwm";
    rev = "7de50ca0f15509af93105738f0d6a6d10c8085b8";
    sha256 = "sha256-/xhK9AAAWwlPcjijh0ZEsMQ5vVEenVNsVxmnYk8SnHU=";
  };

  buildInputs = [ libX11 libXft libXinerama imlib2 ];

  prePatch = ''
    substituteInPlace config.mk --replace '/usr/local' $out
  '';

  postInstall = ''
      mkdir -p $out/bin
      cp scripts/* $out/bin
  '';

  meta = with lib; {
    description = "atar13's fork of dwm";
    homepage = "https://github.com/atar13/dwm";
    license = licenses.mit;
    platforms = platforms.all;
  };
}


{ stdenv, lib, fetchFromGitHub, libX11, libXft, libXinerama, imlib2 }:

stdenv.mkDerivation {
  name = "dwm-atar13";
  version = "0-unstable-2023-04-11";


  src = fetchFromGitHub {
    owner = "atar13";
    repo = "dwm";
    rev = "a7ef295b7fd8d84ce730ba16dde6380bc0e8f206";
    sha256 = "sha256-VgSrYzu3Sxsy7f+MXJZkk3xztcHX/3tud0b4HDI5WqI=";
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


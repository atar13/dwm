{ stdenv, lib, fetchurl, libX11, libXft, libXinerama, imlib2 }:

stdenv.mkDerivation {
  name = "dwm";

  src = builtins.filterSource
    (path: type: (toString path) != (toString ./.git)) ./.;

  buildInputs = [ libX11 libXft libXinerama imlib2 ];

  prePatch = ''
    substituteInPlace config.mk --replace '/usr/local' $out
  '';

  meta = with lib; {
    description = "Dynamic window manager for X";
    homepage = http://dwm.suckless.org/;
    license = licenses.mit;
    platforms = platforms.all;
  };
}


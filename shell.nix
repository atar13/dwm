{ pkgs ? import <nixpkgs> { } }:

with pkgs;
pkgs.mkShell {
  name = "dwm";
  buildInputs = with pkgs; [ xorg.libX11 xorg.libXft xorg.libXinerama imlib2 ];
}

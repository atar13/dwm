{ pkgs ? import <nixpkgs> { } }:

with pkgs;
{
  dwm = pkgs.callPackage ./package.nix { };
}

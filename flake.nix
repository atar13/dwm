{
  description = "atar13's fork of dwm";

  inputs.flake-utils.url = "github:numtide/flake-utils";

  outputs = { self, nixpkgs, flake-utils }:
    flake-utils.lib.eachDefaultSystem
      (system:
        let pkgs = nixpkgs.legacyPackages.${system}; in
        {
          packages.dwm = pkgs.callPackage ./package.nix { };
          devShells.default = import ./shell.nix { inherit pkgs; };
          formatter = pkgs.nixpkgs-fmt;
        }
      );
}


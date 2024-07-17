let
  pkgs = import <nixpkgs> { };
in {
  dwlblocks = pkgs.callPackage ./dwlblocks.nix { };
}

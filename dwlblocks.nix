{ 
    lib,
    stdenv,
    installShellFiles,
    gnumake,
}:

stdenv.mkDerivation {
    name = "dwlblocks";
    src = ./src;

    nativeBuildInputs = [
        installShellFiles
        gnumake
    ];

    buildInputs = [];

    outputs = [ "out" ];

    makeFlags = [
        "PREFIX=$(out)"
    ];

    buildPhase = ''
        make clean
        make
    '';

    meta = {
        homepage = "https://github.com/nullptrexceptions/dwlblocks/";
        description = "A ported version of dwmblocks for dwl/wayland, using stdout";
        license = lib.licenses.gpl2Only;
        mainProgram = "dwlblocks";
    };
}

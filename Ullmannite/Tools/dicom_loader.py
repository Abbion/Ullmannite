import os
import pydicom
import numpy as np

def load_dicom_folder(folder_path):
    dicom_files = []
    for file in os.listdir(folder_path):
        path = os.path.join(folder_path, file)
        try:
            dicom_files.append(pydicom.dcmread(path))
        except Exception:
            pass
    dicom_files.sort(key=lambda d: getattr(d, "InstanceNumber", 0))

    depth = len(dicom_files)
    height, width = dicom_files[3].pixel_array.shape

    volume = np.zeros((depth, height, width), dtype=np.int16)

    for i, ds in enumerate(dicom_files):
        volume[i] = ds.pixel_array

    return width, height, depth, volume

#For tests
def main():
    pass

if __name__ == "__main__":
    main()
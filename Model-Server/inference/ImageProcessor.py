import os
import cv2


class ImageProcessor:
    def __init__(self, class_list=None):
        self.original_image = None
        self.last_annotated_image = None
        self.class_list = class_list if class_list else ["car", "free"]
        self.empty_indices = []
        self.parked_indices = []
        self.all_boxes = []

    def add_image(self, image_path):
        if not os.path.exists(image_path):
            raise FileNotFoundError(f"Image path does not exist: {image_path}")

        image = cv2.imread(image_path)
        if image is None:
            raise ValueError(f"Could not load image from: {image_path}")

        self.original_image = image.copy()
        return image

    def add_image_direct(self, image):
        if image is None or not hasattr(image, 'shape'):
            raise ValueError("Invalid OpenCV image provided.")

        self.original_image = image.copy()

    def sort_boxes_top_to_bottom_left_to_right(self, boxes):
        # Sort boxes top-to-bottom
        boxes = sorted(boxes, key=lambda b: b["coords"][1])  # y_min

        # Estimate average height
        heights = [b["coords"][3] - b["coords"][1] for b in boxes]
        avg_height = sum(heights) / len(heights)
        row_threshold = avg_height * 0.6  # adjust if needed

        rows = []
        for box in boxes:
            y_min = box["coords"][1]
            placed = False
            for row in rows:
                row_y_min = row[0]["coords"][1]
                if abs(y_min - row_y_min) < row_threshold:
                    row.append(box)
                    placed = True
                    break
            if not placed:
                rows.append([box])

        # Now sort each row left-to-right
        sorted_boxes = []
        for row in rows:
            row_sorted = sorted(row, key=lambda b: b["coords"][0])  # x_min
            sorted_boxes.extend(row_sorted)

        return sorted_boxes

    def annotate_image(self, results):
        self.all_boxes.clear()
        self.empty_indices.clear()
        self.parked_indices.clear()
        image = self.original_image.copy()

        for result in results:
            boxes = result.boxes.xyxy
            confidences = result.boxes.conf
            class_ids = result.boxes.cls

            for box, confidence, class_id in zip(boxes, confidences, class_ids):
                x_min, y_min, x_max, y_max = map(int, box)
                self.all_boxes.append({
                    "coords": (x_min, y_min, x_max, y_max),
                    "class_id": int(class_id),
                    "confidence": float(confidence)
                })

        # Sort boxes visually: top-to-bottom, left-to-right
        self.all_boxes = self.sort_boxes_top_to_bottom_left_to_right(self.all_boxes)

        for i, box_info in enumerate(self.all_boxes, start=1):
            x_min, y_min, x_max, y_max = box_info["coords"]
            class_id = box_info["class_id"]

            # Draw box and label
            label = str(i)
            cv2.rectangle(image, (x_min, y_min),
                          (x_max, y_max), (0, 255, 0), 2)
            cv2.putText(image, label, (x_min, y_min - 10),
                        cv2.FONT_HERSHEY_SIMPLEX, 0.6, (200, 255, 0), 2)

            # Append the empty spots
            if class_id == 1:
                self.empty_indices.append(i)
            else:
                self.parked_indices.append(i)

        self.last_annotated_image = image
        return image

    def get_empty_indices(self):
        return self.empty_indices

    def get_all_indices(self):
        return self.all_boxes

    def get_parked_indices(self):
        return self.parked_indices

    def get_parking_summary(self):
        """Returns a summary of total, parked, and empty spots."""
        parked = len(self.get_parked_indices())
        empty = len(self.get_empty_indices())
        total = len(self.get_all_indices())
        empty_indices = self.get_empty_indices()
        parked_indices = self.get_parked_indices()

        return {
            "total_spots": total,
            "parked_count": parked,
            "empty_count": empty,
            "empty_indices": empty_indices,
            "parked_indices": parked_indices,
        }

    def save_and_show(self, output_path="../inference/output/output0.jpg", show=True, resize_dim=(1000, 1000)):
        if self.last_annotated_image is None:
            raise ValueError(
                "No annotated image found. Run annotate_image() first.")

        cv2.imwrite(output_path, self.last_annotated_image)

        if show:
            resized = cv2.resize(self.last_annotated_image, resize_dim)
            cv2.imshow('Parking Detection', resized)
            cv2.waitKey(0)
            cv2.destroyAllWindows()

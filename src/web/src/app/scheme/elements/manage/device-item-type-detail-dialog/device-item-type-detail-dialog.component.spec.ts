import { ComponentFixture, TestBed } from '@angular/core/testing';

import { DeviceItemTypeDetailDialogComponent } from './device-item-type-detail-dialog.component';

describe('DeviceItemTypeDetailDialogComponent', () => {
  let component: DeviceItemTypeDetailDialogComponent;
  let fixture: ComponentFixture<DeviceItemTypeDetailDialogComponent>;

  beforeEach(async () => {
    await TestBed.configureTestingModule({
    imports: [DeviceItemTypeDetailDialogComponent]
})
    .compileComponents();
  });

  beforeEach(() => {
    fixture = TestBed.createComponent(DeviceItemTypeDetailDialogComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});

import { ComponentFixture, TestBed } from '@angular/core/testing';

import { DeviceItemGroupTypeDetailDialogComponent } from './device-item-group-type-detail-dialog.component';

describe('DeviceItemGroupTypeDetailDialogComponent', () => {
  let component: DeviceItemGroupTypeDetailDialogComponent;
  let fixture: ComponentFixture<DeviceItemGroupTypeDetailDialogComponent>;

  beforeEach(async () => {
    await TestBed.configureTestingModule({
      declarations: [ DeviceItemGroupTypeDetailDialogComponent ]
    })
    .compileComponents();
  });

  beforeEach(() => {
    fixture = TestBed.createComponent(DeviceItemGroupTypeDetailDialogComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
